#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "history.h"

#ifndef QUANTIZATION_RADIX_H
#define QUANTIZATION_RADIX_H

typedef struct Node Node;

int node_init_fields(Node *, const char *);
Node * get_child(const Node *, const char *);
Node * tree_find(const Node *, const char *);
Node * tree_init(const char *);
void tree_insert(Node *, const char *);
void node_split(Node *, size_t);
// auxiliary function used to attach a child to a specified parent
void tree_attach(Node *, Node *);

#endif //QUANTIZATION_RADIX_H
