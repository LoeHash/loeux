; 设置程序的起始地址, 如果不设置,则后续的所有寻址全部使用0x00开始
; 也就是会影响绝对寻址功能
; org 就是这样一个设置起始地址的指令
org 0x7c00
        ;实现fat12文件系统
        StackBase               equ     0x7c00  ; 设置栈的起始地址, equ 是一个伪指令, 用来定义常量
        LoaderBase              equ     0x1000   
        LoaderOffset            equ     0x00
        RootDirSectors          equ     14      ; 根目录区占用的扇区数
        SectorNumOfRootDirStart equ     19      ; 根目录区的开始扇区
        SectorNumofFAT1Start    equ     1       ; fat12的fat1表
        ;;;;;;;;;;;;;上面这些都是伪指令, 所以在生成二进制文件时不会占用空间;;;;;;;;;;;;;;;

        jmp     short   _start
        nop     ;纯占位
        ;此处为BPB
        BS_OEMName      db      "loeux :>"
        BPB_BytePerSec  dd      0x0200
        BPB_SecPerClus  db      0x01
        BPB_RsvdSecCnt  dd      0x0001
        BPB_NumFATs     db      0x02
        BPB_RootEntCnt  dd      0x00e0
        BPB_TotSec16    dd      0x0b40
        BPB_Media       db      0xf0
        BPB_FATSz16     dd      0x0009
        BPB_SecPerTrk   dd      0x0012
        BPB_NumHeads    dd      0x0002
        BPB_HiddSec     dw      0x0
        BPB_TotSec32    dw      0x0
        BS_DrvNum       db      0x0
        BS_Reserved1    db      0x0
        BS_BootSig      db      0x29
        BS_VolID        dw      0x0
        BS_VolLab       db      'Loeux Boot'
        BS_FileSysType  db      'FAT12   '


_start:
        ; 现在是实模式, 只有地址线只有 20根, 寄存器只有16位, 所以需要这些段寄存器来实现1mb的寻址空间
        mov ax, cx              ; 此时的cx是0
        mov ds, ax              ; ds	数据段, 默认数据访问, mov ax [1234] 就是访问 ds:1234
        mov es, ax              ; es	附加段
        mov ss, ax              ; ss	栈段, 对于栈的pop,push, 隐式使用 ss:sp

        mov sp, StackBase       ; 栈指针, 与ss配合: ss:sp
        ; 接下来是显示代码
        ; ======清屏
        mov ax, 0600h           ;xxxh 表示xxx是16进制的立即数, ax是16位的寄存器, 其有ah 与 al 分别代表高8位与低8位
        mov bx, 0700h           
        mov cx, 0
        mov dx, 0184fh          
        int 10h                 ;中断, 触发bios的屏幕服务

        ; ======设置焦点, 因为所有的字符打印全部是在焦点位置开始的
        mov ax, 0200h
        mov bx, 0000h
        mov dx, 0000h
        int 10h

        ; ======在屏幕显示: Start Booting...
        mov ax, 1301h
        mov bx, 0x000f
        mov dx, 0000h
        mov cx, 16
        push ax                 ; ax压入栈
        mov ax, ds              ; x86不允许直接 mov es, dx, 必须通过中间变量ax
        mov es, ax              ; x86不允许直接 mov es, dx, 必须通过中间变量ax
        pop ax
        mov bp, StartBootMessage
        int 10h

        ; ======重置磁盘驱动器
        xor ah, ah
        xor dl, dl
        int 13h         ;触发bios磁盘中断服务

        loop:
                jmp loop


        

        ; 函数传参:
        ;       ax:     磁盘起始扇区号
        ;       cl:     读入的扇区数量
        ;       es:bx=> 目标缓冲区的地址(调用前必须设置好!)
        ; 函数输出:
        ;       读取成功: cf进位, 数据在es:bx中
        ;       读取失败: cf进位1
        Func_ReadOneSector:
                ; 这里的目的是开辟栈帧
                ; 在栈上开辟出两个字节的长度(已更改开辟一个字节)
                push    bp
                mov     bp,     sp
                sub     esp,    1

                ; 在这里, 将cl入栈
                ; 但是这里是-2, 但是-1也是可以的, 因为
                ; cl只是一个一个字节,所以我们开辟一个,写一个也完全没问题
                ; cl一会需要存储其他信息
                mov     byte    [bp - 1],       cl
                push    bx

                ; 这里是逻辑扇区号->CHS核心
                mov    bl, [BPB_SecPerTrk]   ; 每磁道扇区数，通常为18
                div    bl                    ; ax ÷ bl → al=商，ah=余数
                inc    ah                    ; 调整为1-based扇区号
                mov    cl, ah                ; cl = 扇区号
                mov    dh, al                ; dh = 磁道号
                shr    al, 1                 ; al = 磁道号÷2 = 柱面号
                mov    ch, al                ; ch = 柱面号
                and    dh, 1                 ; dh = 磁道号最低位 = 磁头号 (0 或 1)
                pop    bx

                mov     dl,     [BS_DrvNum]
        Label_Go_On_Reading:
                
                ; 这里就是设置bios中断服务号的一些
                ; 参数
                mov     ah,     2                       
                mov     al,     byte    [bp - 2]
                int     0x13
                jc      Label_Go_On_Reading
                add     esp, 1                          ; 重置esp    1
                pop     bp                              ; 弹出bp
                ret


        StartBootMessage: db "Start Booting..."



        ; ======结束填充数据
        ; 注意这个填充不会腐干tiems之后的数据
        ; 会把它们往后挤!!!
        times 510 - ($ - $$) db 0aah
        dw 0xaa55

