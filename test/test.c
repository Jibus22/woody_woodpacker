#include <stdio.h>
/*
run.c - a small skeleton program to run shellcode
*/
// bytecode here

/* code avec un appel 'lea' de l'adresse de msg, avec des nullbytes */
/* ben ça fonctionne en fait */
char code[] =
    "\x31\xc0\x99\xb2\x0a\xff\xc0\x89\xc7\x48\x8d\x35\x12\x00\x00\x00\x0f\x05"
    "\xb2\x2a\x31\xc0\xff\xc0\xf6\xe2\x89\xc7\x31\xc0\xb0\x3c\x0f\x05\x2e\x2e"
    "\x57\x4f\x4f\x44\x59\x2e\x2e\x0a";

/* code avec jmp short ender & call starter pour choper l'adresse de la string,
 * mais c'est une adresse relative. ça fonctionne */
char code2[] =
    "\xeb\x1c\x31\xc0\x99\xb2\x0a\xff\xc0\x89\xc7\x5e\x0f\x05\xb2\x2a\x31\xc0"
    "\xff\xc0\xf6\xe2\x89\xc7\x31\xc0\xb0\x3c\x0f\x05\xe8\xdf\xff\xff\xff\x2e"
    "\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x0a";

int main(int argc, char **argv) {
  printf("kjkj\n");
  fflush(stdout);
  int (*func)();           // function pointer
  func = (int (*)())code;  // func points to our shellcode
  (int)(*func)();          // execute a function code[]
  // if our program returned 0 instead of 1,
  // so our shellcode worked
  return 1;
}
