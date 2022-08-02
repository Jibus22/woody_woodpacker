bits 64
default rel
global _start

;     rdi   rsi    rdx
;      v     v      v
;write(fd,   msg,   len);

_start:
            push    rdx
            xor     eax, eax
            xor     edx, edx
            mov     dl, 14          ;3eme argument (rdx)
            inc     eax             ;eax = 1 (syscall)
            mov     edi, eax        ;1er argument rdi = 1
            lea     rsi, [rel msg]  ;2eme arg
            syscall

            pop     rdx
						xor     rax, rax
						add     rax, qword [rsi + (entry - msg)] ;avoid NULL bytes
						push    rax
						ret

msg         db "....WOODY....",10
entry       dq 0xdeadbeefdeadbeef
