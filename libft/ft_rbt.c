#include "ft_rbt.h"

void destroy_rbt(t_rbt *node) {
  if (node) {
    destroy_rbt(node->left);
    destroy_rbt(node->right);
    if (node->value) free(node->value);
    free(node);
  }
}

void print_rbt_inorder(t_rbt *node) {
  if (node) {
    print_rbt_inorder(node->left);
    printf("- %s.\n", (char *)node->value);
    print_rbt_inorder(node->right);
  }
}

/* creates a new node, put value into it and returns its address. */
static void *ft_rbt_new(void *value, t_rbt *parent) {
  t_rbt *new = (t_rbt *)malloc(sizeof(t_rbt) * 1);

  if (!new) return NULL;
  ft_bzero(new, sizeof(t_rbt));
  new->value = value;
  new->color = RBT_RED;
  new->parent = parent;
  return new;
}

static t_rbt *get_uncle(t_rbt *node) {
  if (!node->parent->parent)
    return NULL;
  else if (node->parent->parent->left != node->parent)
    return node->parent->parent->left;
  else
    return node->parent->parent->right;
}

static bool is_llc(t_rbt *node) {
  return (node->parent->parent->left == node->parent &&
          node->parent->left == node);
}

static bool is_lrc(t_rbt *node) {
  return (node->parent->parent->left == node->parent &&
          node->parent->right == node);
}

static bool is_rrc(t_rbt *node) {
  return (node->parent->parent->right == node->parent &&
          node->parent->right == node);
}

static bool is_rlc(t_rbt *node) {
  return (node->parent->parent->right == node->parent &&
          node->parent->left == node);
}

static void right_rotate(t_rbt *node, t_rbt **root) {
  t_rbt *tmp = node->left->right;
  node->left->right = node;
  node->left->parent = node->parent;
  if (!node->parent)
    *root = node->left;
  else if (node->parent->left == node)
    node->parent->left = node->left;
  else
    node->parent->right = node->left;
  node->parent = node->left;
  node->left = tmp;
  if (tmp) tmp->parent = node;
}

static void left_rotate(t_rbt *node, t_rbt **root) {
  t_rbt *tmp = node->right->left;
  node->right->left = node;
  node->right->parent = node->parent;
  if (!node->parent)
    *root = node->right;
  else if (node->parent->left == node)
    node->parent->left = node->right;
  else
    node->parent->right = node->right;
  node->parent = node->right;
  node->right = tmp;
  if (tmp) tmp->parent = node;
}

static void swap_colors(t_rbt *nd1, t_rbt *nd2) {
  short int col_tmp = nd1->color;

  nd1->color = nd2->color;
  nd2->color = col_tmp;
}

static void rbt_balance(t_rbt *node, t_rbt **root) {
  t_rbt *uncle, *gd_parent, *parent = node->parent;

  if (!parent) {
    node->color = RBT_BLACK;
    *root = node;
    return;
  } else if (parent->color == RBT_RED) {
    uncle = get_uncle(node);
    gd_parent = node->parent->parent;
    if (uncle && uncle->color == RBT_RED) {
      uncle->color = RBT_BLACK;
      parent->color = RBT_BLACK;
      gd_parent->color = RBT_RED;
      rbt_balance(gd_parent, root);
    } else if (!uncle || uncle->color == RBT_BLACK) {
      if (is_llc(node))
        right_rotate(gd_parent, root);
      else if (is_lrc(node)) {
        left_rotate(parent, root);
        node = parent;
        parent = node->parent;
        right_rotate(gd_parent, root);
      } else if (is_rrc(node))
        left_rotate(gd_parent, root);
      else if (is_rlc(node)) {
        right_rotate(parent, root);
        node = parent;
        parent = node->parent;
        left_rotate(gd_parent, root);
      }
      swap_colors(gd_parent, parent);
      rbt_balance(parent, root);
    }
  }
}

t_rbt *ft_rbt_insert(t_rbt *node, void *value,
                     int compare(const void *, const void *)) {
  int comp;
  t_rbt *new, *parent = node, *root = node;

  while (node) {
    parent = node;
    comp = compare(node->value, value);
    if (comp == 0) {
      node->nb += 1;
      return root;
    } else if (comp > 0)
      node = node->left;
    else if (comp < 0)
      node = node->right;
  }
  new = ft_rbt_new(value, parent);
  if (parent) {
    if (comp > 0)
      parent->left = new;
    else
      parent->right = new;
  }
  rbt_balance(new, &root);
  return root;
}
