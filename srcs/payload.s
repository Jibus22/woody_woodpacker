bits 64
global _start

;     rdi   rsi    rdx
;      v     v      v
;write(fd,   msg,   len);

_start:
      push   rdx             ;dunno why it segf if I don't push/pop rdx
      
      lea    rsi, [rel msg]  ;2eme arg
      xor    rdi, rdi
      
      lea    rax, [rsi + (_start - msg)]
      mov    r11, rax
      mov    rdi, rax
      sub    r11, [rsi + (text_offset - msg)]
      sub    rax, [rsi + (entry_offset - msg)]
      push   rax

      mov r8, [rsi + (segment_offset - msg)]
      push rsi
      push r11 ;syscall alter r11
      mov rax, 10
      sub rdi, r8
      mov rsi, r8
      mov rdx, 0x7
      syscall
      pop r11
      pop rsi

      xor    rcx, rcx
      xor    rdx, rdx
      
      mov    r8, [rsi + (text_len - msg)]; r8 = len (payload - main)
      lea    r9, [rsi + (key - msg)]
      
      test_loop:
      cmp    rcx, r8  ;if whole code is roamed go to output (while(i< len) i++)
      je     output
      cmp    rdx, [rsi + (key_size - msg)] ; if j < key_size, j++, else j=0
      jl     continue
      xor    rdx, rdx
      continue:
      mov    r10b, byte[r9 + rdx]
      xor    byte[r11 + rcx], r10b
      inc    rcx
      inc    rdx
      jmp    test_loop
      
      output:
      
      xor    eax, eax
      xor    edx, edx
      inc    eax             ;eax = 1 (linux write syscall)
      mov    edi, eax        ;1er argument rdi = 1
      mov    dl, 14          ;3eme argument (rdx)
      syscall
      
      pop    rax
      pop    rdx             ;dunno why it segf if I don't push/pop rdx
      
      push   rax
      ret

msg             db "....WOODY....",10
entry_offset    dq 0x1a1b2a2b3a3b4a4b
text_offset     dq 0x1a1b2a2b3a3b4a4b
segment_offset  dq 0x1a1b2a2b3a3b4a4b
text_len        dq 0x1a1b2a2b3a3b4a4b
key_size        dq 0x3333333322222222
key             times 64 db 0x42
