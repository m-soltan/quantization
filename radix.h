#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "energy.h"
#include "history.h"

#ifndef QUANTIZATION_RADIX_H
#define QUANTIZATION_RADIX_H

typedef struct Node Node;

Energy * get_energy(const Node *);
/*
 * let p - longest prefix of the string parameter that exists in the tree
 * N - the last node that contains some part of p
 * tree_find returns a pointer to N's parent
 */
const Node * tree_find(const Node *, const char *);
Node * tree_find_split(Node *, const char *);
Node * tree_init();
void tree_destroy(Node **);
void tree_destroy_rec(Node **);
void tree_insert(Node *, const char *);
// removes the whole subtree under string parameter
void tree_remove(Node *, const char *);

// debug functions - no side effects, only called in assertions
int debug_single_child(const Node *);

#endif //QUANTIZATION_RADIX_H
