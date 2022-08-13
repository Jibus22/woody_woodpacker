#include "woody.h"

static int check_elf_ident(const char *file) {
  return ((ft_memcmp(file, ELFMAG, SELFMAG) != 0) +
          (file[EI_CLASS] <= 0 || file[EI_CLASS] > 2) +
          (file[EI_DATA] <= 0 || file[EI_DATA] > 2) +
          (file[EI_VERSION] != EV_CURRENT));
}

static t_ret injection_x32() { return ret_wrap(0, 0, NULL); }

int main(int ac, char **av) {
  int fd;
  t_ret ret;
  off_t size = 0;
  void *file;

  if (ac != 2)
    return exit_error(ret_wrap(OOPS_BAD_ARG_NB, 0, NULL), -1, size, av[0]);
  if ((fd = open(av[ac - 1], O_RDONLY)) == -1)
    return exit_error(ret_wrap(OOPS_OPEN, 0, NULL), -1, size, av[0]);
  size = lseek(fd, 0, SEEK_END);
  if (size == -1)
    return exit_error(ret_wrap(OOPS_LSEEK, 0, NULL), fd, size, av[0]);
  file = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
  if (file == MAP_FAILED)
    return exit_error(ret_wrap(OOPS_MMAP, 0, NULL), fd, size, av[0]);
  close(fd);

  if (size < 16 || check_elf_ident(file) > 0)
    return exit_error(ret_wrap(OOPS_NOTELF, size, file), -1, size, av[0]);
  if (((char *)file)[EI_CLASS] == ELFCLASS64)
    ret = injection_x64(file, size);
  else
    ret = injection_x32();
  if (ret.err) return exit_error(ret, -1, size, av[0]);

  return EXIT_SUCCESS;
}
