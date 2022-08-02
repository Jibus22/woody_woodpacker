bits 64
default rel
global _start

;     rdi   rsi    rdx
;      v     v      v
;write(fd,   msg,   len);

_start:
            xor     eax, eax
            cdq
            mov     dl, 10 ;3eme argument (rdx)
            inc     eax ; eax = 1 (syscall)
            mov     edi, eax ;1er argument rdi = 1
            lea     rsi, [rel msg] ;2eme arg
            syscall
						mov dl, 42   ;petit registre pour enregistrer 42 sans 0x00
						xor eax, eax
						inc eax
						mul dl       ;multiplier 42 * rax (rax = 1), resultat dans rax
						mov edi, eax ;bouger la valeur 42 de rax à rdi
						xor eax, eax
						mov al, 60 ;on met le syscall exit dans rax
						syscall

msg         db "..WOODY..",10
