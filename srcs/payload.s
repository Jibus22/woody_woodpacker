bits 64
global _start

_start:
  push rdx ;dunno why it segf if I don't push/pop rdx
  
  lea  r8, [rel msg] ;REFERENCE
  lea  r9, [r8 + (_start - msg)] ;PAYLOAD
  
  xor  eax, eax
  xor  edx, edx
  mov  al, 0xa
  mov  rdi, r9
  sub  rdi, [r8 + (segment_offset - msg)]
  mov  rsi, [r8 + (segment_offset - msg)]
  mov  dl, 0x7
  syscall
  
  mov  rdi, r9
  sub  rdi, [r8 + (text_offset - msg)] ;TEXT
  sub  r9, [r8 + (entry_offset - msg)] ;ENTRY
  
  xor  rax, rax ;KEY INDEX
  xor  rcx, rcx ;TEXT INDEX
  mov  rdx, [r8 + (text_len - msg)] ;TEXT LEN
  lea  rsi, [r8 + (key - msg)] ;KEY
  
roam_text:
  cmp rcx, rdx
  je  output
  cmp rax, [r8 + (key_size - msg)]
  jl  uncrypt
  xor rax, rax
uncrypt:
  mov  r10b, byte[rsi + rax]
  xor  byte[rdi + rcx], r10b
  inc  rax
  inc  rcx
  jmp  roam_text
  
output:
  xor  eax, eax
  xor  edx, edx
  inc  eax
  mov  edi, eax
  mov  rsi, r8
  mov  dl, 0xe
  syscall
  
  pop  rdx
  push r9
  ret

msg             db "....WOODY....",10
entry_offset    dq 0x1a1b2a2b3a3b4a4b
text_offset     dq 0x1a1b2a2b3a3b4a4b
segment_offset  dq 0x1a1b2a2b3a3b4a4b
text_len        dq 0x1a1b2a2b3a3b4a4b
key_size        dq 0x3333333322222222
key             times 64 db 0x42
