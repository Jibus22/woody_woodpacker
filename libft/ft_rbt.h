#ifndef FT_RBT_H
#define FT_RBT_H

#include <stdbool.h>
#include <stdio.h>

#include "libft.h"

#define RBT_RED 0
#define RBT_BLACK 1

typedef struct s_rbt {
  void *value;
  struct s_rbt *left;
  struct s_rbt *right;
  struct s_rbt *parent;
  short int nb;
  short int color;
} t_rbt;

t_rbt *ft_rbt_insert(t_rbt *node, void *value,
                     int compare(const void *, const void *));
void print_rbt_inorder(t_rbt *node);
void destroy_rbt(t_rbt *node);

#endif
