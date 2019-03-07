#include "radix.h"

/* the size of the alphabet - the digits 0, 1, 2, 3 (4) plus 1 for the 5th
 * place, reserved for a marker
 */
#define DEGREE 5

int node_fit(const Node *x, const char *pattern) {
	return (!strncmp(x->val.val, pattern, x->val.len - 1));
}

int node_init_fields(Node *x, const char *s) {
	Node ***children_tmp = (Node ***) &x->children;
	*children_tmp = (Node **) malloc(DEGREE * sizeof(Node *));
	if (*children_tmp) {
		History h = hist_init(s);
		size_t *len_tmp = (size_t *) &x->val.len;
		const char **val_tmp = (const char **) &x->val.val;
		*len_tmp = h.len;
		*val_tmp = h.val;
		return 0;
	}
	return 1;
}

Node *tree_find(const Node *tree, const char *pattern) {
	// dummy value for the first iteration
	const Node *prev = tree;
	while (tree && node_fit(tree, pattern)) {
		prev = tree;
		tree = tree->children[*pattern - '0'];
		pattern += prev->val.len - 1;
	}
	return (Node *) prev;
}

Node * tree_init(const char *s) {
	Node *ans = (Node *) malloc(sizeof(Node));
	if (ans) {
		if (!node_init_fields(ans, s)) {
			for (size_t i = 0; i < DEGREE; ++i)
				ans->children[i] = NULL;
			return ans;
		}
		free(ans);
	}
	return NULL;
}

void tree_attach(Node *parent, Node *child) {
	int position = child->val->val[0] - '0';
	size_t *depth_tmp = (size_t *) &child->depth;
	*depth_tmp = parent->depth + parent->val->len - 1;
	if (!parent->children[position])
		parent->children[position] = child;
}

