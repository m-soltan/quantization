#ifndef QUANTIZATION_TRIE_H
#define QUANTIZATION_TRIE_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "energy.h"

typedef struct Node Node;

Energy **get_energy(Node *);
int tree_insert(Node *, const char *);
int tree_remove(Node *, const char *);
int add_energy(Node *, energy_t);
void tree_finish(void);
Node *tree_find(Node *, const char *);
Node *tree_init(void);
int tree_destroy(Node **);

#endif //QUANTIZATION_TRIE_H
