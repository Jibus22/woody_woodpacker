#include <stdio.h>
#include <unistd.h>

/* this is a file to test executable shared library */
/* It must be compiled with: */
/* clang -shared hw.c -o libhello.so -Wl,-soname,libhello.so -Wl,-e,lib_entry */

const char prout[] __attribute__((section(".interp"))) = "/lib64/ld-linux-x86-64.so.2";

int hw() {
	printf("Hello world!\n");
	return 0;
}

void lib_entry() {
	printf("entry of libhello\n");
	_exit(0);
}
