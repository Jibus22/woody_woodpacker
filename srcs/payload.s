bits 64
global _start

;     rdi   rsi    rdx
;      v     v      v
;write(fd,   msg,   len);

_start:
            push	  rdx             ;dunno why it segf if I don't push/pop rdx

            xor     eax, eax
            xor     edx, edx
            mov     dl, 14          ;3eme argument (rdx)
            inc     eax             ;eax = 1 (syscall)
            mov     edi, eax        ;1er argument rdi = 1
            lea     rsi, [rel msg]  ;2eme arg
            syscall

            pop	    rdx

						lea     rax, [rsi + (_start - msg)]        ;tricky maths to avoid
						sub     rax, [rsi + (payload_entry - msg)] ;NULL bytes. rsi == msg
						add     rax, [rsi + (main_entry - msg)]    ;Calculates the relative
						push    rax                                ;main entry point
						ret

msg           db "....WOODY....",10
main_entry    dq 0x1a1b2a2b3a3b4a4b
payload_entry dq 0x5a5b6a6b7a7b8a8b
