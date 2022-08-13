#include "woody.h"

t_ret ret_wrap(int err, unsigned int size, void *file) {
  t_ret ret = {err, size, file};
  return ret;
}

int oops_error(unsigned int err) {
  static struct {
    int err_nb;
    char oops_msg[OOPS_SIZE];
  } oops[OOPS_NB] = {
      {OOPS_UND, "undefined err\n\0"},
      {OOPS_OPEN, "open() error\n\0"},
      {OOPS_LSEEK, "lseek() error\n\0"},
      {OOPS_MMAP, "mmap() failed\n\0"},
      {OOPS_NOTELF, "file isn't ELF\n\0"},
      {OOPS_NOCAVE, "no cave found\n\0"},
      {OOPS_BAD_ARG_NB, "wrong number of arguments\n\0"},
      {OOPS_BAD_ELF, "elf not executable or corrupted\n\0"},
      {OOPS_NO_LOAD, "no LOAD segment found\n\0"},
      {OOPS_BAD_PHDR, "program header corrupted\n\0"},
      {OOPS_BAD_SHDR, "section header corrupted\n\0"},
      {OOPS_NO_TEXT, "no .text with expected permissions found\n\0"},
      {OOPS_READ, "read() error\n\0"},
  };

  if (err > OOPS_NB) err = OOPS_UND;
  write(STDERR_FILENO, oops[err].oops_msg, ft_strlen(oops[err].oops_msg));
  return EXIT_FAILURE;
}

int exit_error(t_ret err, int fd, off_t filesize, char *name) {
  if (fd > -1) close(fd);
  if (err.file && filesize > 0) {
    if (err.size == filesize)
      munmap(err.file, err.size);
    else
      free(err.file);
  }
  write(STDERR_FILENO, name, ft_strlen(name));
  write(STDERR_FILENO, ": ", 2);
  if (err.err >= 0)
    return oops_error(err.err);
  else
    return EXIT_FAILURE;
}
