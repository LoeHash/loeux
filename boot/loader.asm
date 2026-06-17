; loader的基本地址 必须是0x10000
org 0x10000
        
        mov ax, cx            
        mov ds, ax            	
        mov es, ax            	
        mov ax, 0x00
        mov ss, ax            	
        mov sp, 0x7c00
        
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
        mov bp, StartLoadingMessage
        int 10h


StartLoadingMessage: db "Start Loading the kernel..."