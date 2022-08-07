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
       
       
       lea     rax, [rsi + (_start - msg)] ; rax = l'adresse de _start
       xor     r8, r8
       add     r8, [rsi + (payload_entry - msg)]
       sub     r8, [rsi + (main_entry - msg)]; r8 = len (payload - main)
       sub     rax, r8 ; rax = _start - len (= début du code pgm, en mémoire)

			 xor     rcx, rcx ; i = 0
			 xor     rdx, rdx ; j = 0
			 thisloop:
			 cmp     rcx, r8   ; if i < len, go to thisloop (while (i< len) i++)
			 jl      encrypt
			 jmp     end
			 encrypt:

			 inc     rcx

			 end:

       pop     rdx
       push    rax
       ret

msg           db "....WOODY....",10
main_entry    dq 0x1a1b2a2b3a3b4a4b
payload_entry dq 0x5a5b6a6b7a7b8a8b
key_size      dq 0x3333333322222222
key           db "4242424242424242424242424242424242424242424242424242424242424242"



; start - (payload_e - main_entry)
; start - payload_e + main_entry
