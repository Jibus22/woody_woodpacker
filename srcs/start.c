#include "woody.h"

static int check_elf_ident(const char *file) {
  return ((ft_memcmp(file, ELFMAG, SELFMAG) != 0) +
          (file[EI_CLASS] <= 0 || file[EI_CLASS] > 2) +
          (file[EI_DATA] <= 0 || file[EI_DATA] > 2) +
          (file[EI_VERSION] != EV_CURRENT));
}

static int injection_x32() { return 0; }

int main(int ac, char **av) {
  int fd;
  unsigned int ret;
  off_t size;
  void *file;

  if (ac != 2) return exit_error(OOPS_BAD_ARG_NB, -1, NULL, 0, av[0]);
  if ((fd = open(av[ac - 1], O_RDONLY)) == -1)
    return exit_error(OOPS_OPEN, -1, NULL, 0, av[0]);
  size = lseek(fd, 0, SEEK_END);
  if (size == -1) return exit_error(OOPS_LSEEK, fd, NULL, 0, av[0]);
  file = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
  if (file == MAP_FAILED) return exit_error(OOPS_MMAP, fd, NULL, 0, av[0]);
  close(fd);

  if (size < 16 || check_elf_ident(file) > 0)
    return exit_error(OOPS_NOTELF, -1, file, size, av[0]);
  if (((char *)file)[EI_CLASS] == ELFCLASS64)
    ret = injection_x64(file, size);
  else
    ret = injection_x32();
  if (ret)
    return exit_error(ret, -1, file, size, av[0]);

  if ((fd = open("woody", O_RDWR | O_TRUNC | O_CREAT, 0744)) == -1)
    return exit_error(OOPS_OPEN, -1, file, size, av[0]);
  write(fd, file, size);
  munmap(file, size);
  close(fd);
  return EXIT_SUCCESS;
}
