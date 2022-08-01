#include "ft_rbt.h"

#include <string.h>
#include <time.h>

#define STR_NB 250

int strcmp_wrapper(const void *a, const void *b) {
  char *ac = (char *)a;
  char *bc = (char *)b;
  return strcmp(ac, bc);
}

int main() {
  char alnum[62] = {
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"};
  char buf[STR_NB][31];

  bzero(buf, sizeof(buf));
  srand(time(0));
  for (int i = 0; i < STR_NB - 2; i++) {
    for (int j = 0; j < 30; j++) {
      buf[i][j] = alnum[(rand() % 61)];
    }
  }

  /* _____________________________________________________________________ */

  t_rbt *root = NULL;

  for (int i = 0; i < STR_NB; i++) {
    /* printf("(%d)\n", i); fflush(stdout); */
    root = ft_rbt_insert(root, buf[i], strcmp_wrapper);
  }

  print_rbt_inorder(root);

  printf("this is the root: %s.\n", (char *)root->value);

  /* destroy_rbt(root); */

  /* system("leaks a.out"); */

  return EXIT_SUCCESS;
}
