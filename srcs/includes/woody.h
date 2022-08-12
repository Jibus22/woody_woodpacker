#ifndef WOODY_H
#define WOODY_H

#include <elf.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "libft.h"

#ifndef PAYLOAD
#define PAYLOAD ""
#endif

#define PAYLOAD_SIZE (sizeof(PAYLOAD) - 1)

#define KEYLEN 64

#define OOPS_NB 13
#define OOPS_SIZE 48

typedef enum {
  OOPS_UND = 0,
  OOPS_OPEN,
  OOPS_LSEEK,
  OOPS_MMAP,
  OOPS_NOTELF,
  OOPS_NOCAVE,
  OOPS_BAD_ARG_NB,
  OOPS_BAD_ELF,
  OOPS_NO_LOAD,
  OOPS_BAD_PHDR,
  OOPS_BAD_SHDR,
  OOPS_NO_TEXT,
  OOPS_READ,
} e_oops;

typedef struct s_woody {
  Elf64_Phdr *load_seg;
  Elf64_Shdr *text_sec;
} t_woody;

typedef struct s_patch {
  Elf64_Off entry_offset; /* offset from payload to pgm entrypoint */
  Elf64_Off text_offset; /* offset from payload to text entrypoint */
  Elf64_Off segment_offset; /* offset from payload to beginning of segment */
  Elf64_Off text_len;
  Elf64_Off key_size;
  char key[KEYLEN];
} t_patch;

/* injection_x64 */
unsigned int injection_x64(Elf64_Ehdr *file, const int filesize);

/* get_elf_data.c */
unsigned int get_load_segment(const Elf64_Ehdr *file, const int filesize,
                              t_woody *woody);
unsigned int get_text_section(const Elf64_Ehdr *file, const int filesize,
                              t_woody *woody);

/* error.c */
int exit_error(int err, int fd, void *file, size_t size, char *name);
int oops_error(unsigned int err);

#endif
