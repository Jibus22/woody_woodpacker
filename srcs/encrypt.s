global encrypt

section .text

struc mypatch
  entry_off: resq 1
  text_off: resq 1
  segment_off: resq 1
  text_len: resq 1
  key_size: resq 1
  key: resb 64
endstruc

; encrypt2(char *ptr, unsigned long len, const t_patch *patch)
;              rdi,                 rsi,                rdx

encrypt:
xor r8, r8 ;i
xor r9, r9 ;j
roam_text:
cmp r8, rsi
je end
cmp r9, [rdx + mypatch + key_size]
jl crypt
xor r9, r9
crypt:
mov r10b, [rdx + mypatch + key + r9]
xor byte [rdi + r8], r10b
inc r8
inc r9
jmp roam_text
end:
ret
