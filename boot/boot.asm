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
        FatBufferBase           equ     0x7e00  ; fat缓冲区的起始地址
        FatSecTotSize           equ     32
        FatRootDirStartAddr     equ     0x8280  ; 根目录区域开始地址
        SectorBalance           equ     17
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

        ; 加载fat相关文件
        Lable_Load_Fat_Info:
                mov ax, 1
                mov cl, FatSecTotSize
                mov bx, FatBufferBase
                call Func_ReadSectors


        ; 接下来加载内核文件
        mov si, KernelFileName
        call Func_FindFileByName

        ; 现在我们找到了加载器文件的第一个簇号
        ; 然后我们直接加载文件到内存
        Label_FileName_Found:

	mov	ax,	RootDirSectors
	and	di,	0ffe0h
	add	di,	01ah
	mov	cx,	word	[es:di]
	push	cx
	add	cx,	ax
	add	cx,	SectorBalance
	mov	ax,	LoaderBase
	mov	es,	ax
	mov	bx,	LoaderOffset
	mov	ax,	cx

        Label_Go_On_Loading_File:
                push	ax
                push	bx
                mov	ah,	0eh
                mov	al,	'.'
                mov	bl,	0fh
                int	10h
                pop	bx
                pop	ax

                mov	cl,	1
                call	Func_ReadSectors
                pop	ax
                call	Func_GetFATEntry
                cmp	ax,	0fffh
                jz	Label_File_Loaded
                push	ax
                mov	dx,	RootDirSectors
                add	ax,	dx
                add	ax,	SectorBalance
                add	bx,	[BPB_BytePerSec]
                jmp	Label_Go_On_Loading_File
        Label_File_Loaded:
                jmp	LoaderBase:LoaderOffset         ; 执行交接
        

        ; 参数: 
        ; ah=fat表项号
        Func_GetFATEntry:
                push    es
                push    bx
                push    ax
                mov     ax, 00
                mov     es, ax
                pop     ax
                mov     byte    [Odd], 0
                mov     bx,     3
                mul     bx
                mov     bx,     2
                div     bx
                cmp     dx,     0
                jz      Label_Even
                mov     byte    [Odd],  1
        
        Label_Even:
                xor     dx,     dx
                mov     bx,     [BPB_BytePerSec]
                div     bx
                push    dx
                mov     bx,     8000h
                add     ax,     SectorNumofFAT1Start
                mov     cl,     2
                call    Func_ReadSectors
                pop     dx
                add     bx,     dx
                mov     ax,     [es:bx]
                cmp     byte    [Odd],  1
                jnz     Label_Even_2
                shr     ax,     4
        
        Label_Even_2:
                and     ax,     0fffh
                pop     bx
                pop     ES
                ret


        ; 实现fat12文件系统的根据文件名寻找文件
        ; 函数传参
        ; si 文件名字符串的地址
        ; 函数返回
        ; 把逻辑扇区号设置到bx中
        Func_FindFileByName:
                ; ax入栈, 一会要作为指针使用
                push ax      
                push cx
                push si
                push di
                mov ax, FatRootDirStartAddr 
                mov bx, ax
                mov cx, 11
                mov di, ax
                Label_FindFileByName_FindLoop:
                        ; 直接找到根目录区的位置
                        ; 读取11字节
                        cmp byte [di], 0
                        je Label_FindFileByName_NotFound
                        ; 保存当前目录项指针（DI）和文件名指针（SI）
                        push si
                        push di
                        ; 比较11字节：DS:SI（文件名） vs ES:DI（目录项），ES需与DS相同
                        cld
                        repe cmpsb
                        pop di           ; 恢复目录项指针
                        pop si           ; 恢复文件名指针
                        je Label_FindFileByName_Found
                        ; 不相同，前进到下一个目录项（32字节）
                        add di, 32
                        jmp Label_FindFileByName_FindLoop

                Label_FindFileByName_Found:
                        ; DI 指向找到的目录项首地址
                        mov bx, [di + 0x1A]   ; 读取2字节起始簇号（实际只用低12位，但FAT12中高4位为0）
                        and bx, 0x0FFF        ; 确保只取低12位（安全起见）
                        pop di
                        pop cx
                        pop si
                        pop ax
                        clc              ; 清除进位表示成功
                        ret
                Label_FindFileByName_NotFound:
                        mov ax, 1301h
                        mov bx, 0x008c
                        mov dx, 0100h
                        mov cx, 28
                        push ax                 
                        mov ax, ds               
                        mov es, ax               
                        pop ax
                        mov bp, NoLoadMessage
                        int 10h
                        jmp $
                
        ; 函数传参:
        ;       ax:     磁盘起始扇区号
        ;       cl:     读入的扇区数量
        ;       es:bx=> 目标缓冲区的地址(调用前必须设置好!)
        ; 函数输出:
        ;       读取成功: cf进位, 数据在es:bx中
        ;       读取失败: cf进位1
        Func_ReadSectors:
                ; 这里的目的是开辟栈帧
                ; 在栈上开辟出两个字节的长度(已更改开辟一个字节)
                push    bp
                mov     bp,     sp
                sub     sp,    1

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
                mov     al,     byte    [bp - 1]
                int     0x13
                jc      Label_Go_On_Reading
                add     sp, 1                          ; 重置esp    1
                pop     bp                              ; 弹出bp
                ret


        StartBootMessage: db "Start Booting..."
        KernelFileName:   db "KERNEL  BIN",0
        NoLoadMessage:    db "The kernel.bin is not found!"
        Odd:              db 0
        ; ======结束填充数据
        ; 注意这个填充不会腐干tiems之后的数据
        ; 会把它们往后挤!!!
        times 510 - ($ - $$) db 0aah
        dw 0xaa55