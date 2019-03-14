#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "energy.h"
#include "history.h"

#ifndef QUANTIZATION_RADIX_H
#define QUANTIZATION_RADIX_H

typedef struct Node Node;

int node_init_fields(Node *, const char *);
Node * get_child(const Node *, const char *);
Node ** get_child_ref(const Node *parent, const char *pattern);
Node * node_init(const char *);
/*
 * finds the longest prefix of the string parameter in the tree and returns
 * a pointer to its last node
 */
Node * tree_find(const Node *, const char *);
Node * tree_init();
void node_absorb(Node *x, size_t);
void node_split(Node *, size_t);
// auxiliary function used to attach a child to a specified parent
void tree_attach(Node *, Node *);
void tree_destroy(Node **);
void tree_destroy_rec(Node **);
void tree_insert(Node *, const char *);
// removes the whole subtree
void tree_remove(Node *, const char *);

// debug functions - no side effects, only called in assertions
int debug_single_child(const Node *);

#endif //QUANTIZATION_RADIX_H
