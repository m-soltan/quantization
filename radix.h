#include <stdlib.h>
#include <string.h>
#include "history.h"

#ifndef QUANTIZATION_RADIX_H
#define QUANTIZATION_RADIX_H

typedef History * Old_history;

typedef struct Node {
	History val;
	const size_t depth;
	struct Node const** children;
} Node;

int node_init_fields(Node *, const char *);
Node * tree_find(const Node *, const char *);
Node * tree_init(const char *);
void tree_attach(Node *, Node *);

#endif //QUANTIZATION_RADIX_H
