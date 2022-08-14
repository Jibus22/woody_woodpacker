#include "woody.h"

static void buf_utox(char *str, unsigned int n) {
  unsigned int e = n / 16;
  short int res;
  int i = 1;

  for (; e; i++) e /= 16;
  str[i] = 0;
  while (i--) {
    res = ((n / ft_pow(16, e++)) % 16);
    res = ((res + 48) * (res < 10)) + ((res + 87) * (res >= 10));
    str[i] = res;
  }
}

void print_key(const unsigned char *key, unsigned int size) {
  char buf[320];
  char buf2[8];
  int j;

  j = ft_strlcpy(buf, "KEY=\n", 320);
  for (unsigned int i = 0; i < size; i++) {
    if (i && i % 8 == 0) j += ft_strlcpy(buf + j, "\n", 320);
    j += ft_strlcpy(buf + j, "\\x", 320);
    buf_utox(buf2, (unsigned int)key[i]);
    j += ft_strlcpy(buf + j, buf2, 320);
  }
  j += ft_strlcpy(buf + j, "\n", 320);
  write(STDOUT_FILENO, buf, j);
}

static void buf_ultoa(char *str, unsigned long int n) {
  unsigned long int e = n / 10;
  int i = 1;

  for (; e; i++) e /= 10;
  str[i] = 0;
  while (i--) str[i] = ((n / ft_pow(10, e++)) % 10) + 48;
}

void print_info(int ret) {
  int i;
  char buf[320];
  char buf2[320];

  i = ft_strlcpy(buf, "cave: ", 320);
  buf_ultoa(buf2, ret);
  i += ft_strlcpy(buf + i, buf2, 320);
  i += ft_strlcpy(buf + i, " - size of payload: ", 320);
  buf_ultoa(buf2, PAYLOAD_SIZE);
  i += ft_strlcpy(buf + i, buf2, 320);
  i += ft_strlcpy(buf + i, " - size of patch: ", 320);
  buf_ultoa(buf2, sizeof(t_patch));
  i += ft_strlcpy(buf + i, buf2, 320);
  i += ft_strlcpy(buf + i, "\n", 320);
  write(STDOUT_FILENO, buf, i);
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
