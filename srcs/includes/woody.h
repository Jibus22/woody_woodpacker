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

#define OOPS_NB 12
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
} e_oops;

typedef struct s_patch {
  Elf64_Off main_entry;
  Elf64_Off payload_entry;
}              t_patch;

/* error.c */
int exit_error(int err, int fd, void *file, size_t size, char *name);
int oops_error(unsigned int err);

/* injection_x64 */
unsigned int injection_x64(Elf64_Ehdr *file, const int filesize);

#endif
