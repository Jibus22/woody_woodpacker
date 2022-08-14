#include "woody.h"

void print_key(const unsigned char *key, unsigned int size) {
  printf("KEY=\n");
  for (unsigned int i = 0; i < size; i++) {
    if (i && i % 8 == 0) printf("\n");
    printf("\\x%x", (int)key[i]);
  }
  printf("\n");
}

int get_random_key(unsigned char *key) {
  int fd, ret;

  fd = open("/dev/urandom", O_RDONLY);
  if (fd == -1) return OOPS_OPEN;
  ret = read(fd, key, KEYLEN);
  if (ret == -1) return OOPS_READ;
  close(fd);
  return 0;
}
