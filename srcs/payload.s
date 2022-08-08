bits 64
global _start

;     rdi   rsi    rdx
;      v     v      v
;write(fd,   msg,   len);

_start:
       push	   rdx             ;dunno why it segf if I don't push/pop rdx
       
       lea     rsi, [rel msg]  ;2eme arg
       
			 xor     rax, rax
       lea     rax, [rsi + (_start - msg)] ; rax = l'adresse de _start
			 xor     r8, r8
       add     r8, qword [rsi + (payload_entry - msg)]
       sub     r8, qword [rsi + (main_entry - msg)]; r8 = len (payload - main)
       sub     rax, r8 ; rax = _start - len (= début du code pgm, en mémoire)
			 push    rax; save it

			 xor     rcx, rcx ; i = 0
			 xor     rdx, rdx ; j = 0
			 xor     r9, r9
			 xor     r10, r10


;mov rdi, rax
;mov rax, 10 ; mprotect
;mov rsi, r8
;mov rdx, 0x7 ; PROT_READ | PROT_WRITE | PROT_EXEC
;syscall

			 lea     r9, [rel key]
			 test_loop:
			 cmp     rcx, r8  ;if whole code is roamed go to output (while(i< len) i++)
			 je      output
			 cmp     rdx, [rsi + (key_size - msg)] ; if j < key_size, j++, else j=0
			 jl      continue
			 xor     rdx, rdx
			 continue:
			 mov     r10b, byte[r9 + rdx]
			 ;mov     r9b, byte[rel key + rdx] ; r9b = key[j]
			 xor     byte[rax + rcx], r10b
			 inc     rcx
			 inc     rdx
			 jmp     test_loop

			 output:

       xor     eax, eax
       xor     edx, edx
			 xor     rdi, rdi ; j = 0
       inc     eax             ;eax = 1 (linux write syscall)
       mov     edi, eax        ;1er argument rdi = 1
       mov     dl, 14          ;3eme argument (rdx)
       syscall
       
       pop     rax
       pop     rdx
       push    rax
       ret

msg           db "....WOODY....",10
main_entry    dq 0x1a1b2a2b3a3b4a4b
payload_entry dq 0x5a5b6a6b7a7b8a8b
key_size      dq 0x3333333322222222
key           db "4242424242424242424242424242424242424242424242424242424242424242"
