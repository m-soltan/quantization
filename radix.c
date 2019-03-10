#include "radix.h"

/*
 * the size of the alphabet - the 4 digits 0, 1, 2, 3 plus 1 for the 5th
 * place, reserved for a marker
 */
#define DEGREE 5

struct Node {
	History his;
	size_t depth;
	struct Node **children;
};

int node_fit(const Node *x, const char *pattern) {
	return (!strncmp(x->his.val, pattern, x->his.len - 1));
}

int node_init_fields(Node *x, const char *s) {
	x->children = (Node **) malloc(DEGREE * sizeof(Node *));
	if (x->children) {
		x->depth = 0;
		hist_init(&x->his, s);
		return 0;
	}
	return 1;
}

Node * get_child(const Node *parent, const char *pattern) {
	assert(pattern[0] >= '0' && pattern[0] < '4');
	return parent->children[pattern[0] - '0'];
}

Node * tree_find(const Node *tree, const char *pattern) {
	const Node *next = get_child(tree, pattern);
	for (; next && node_fit(tree, pattern); next = get_child(tree, pattern)) {
		pattern += tree->his.len - 1;
		tree = next;
	}
	return (Node *) tree;
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

void tree_insert(Node *tree, const char *str) {
	Node *parent = tree_find(tree, str);
	Node *child = tree_init(str + parent->depth + parent->his.len - 1);
	tree_attach(parent, child);
}

void node_split(Node *x, size_t prefix_size) {
	History old = x->his;
	Node *added_child = tree_init(NULL);
	x->his = hist_prefix(&old, prefix_size);
	added_child->his = hist_suffix(&old, old.len - prefix_size + 1);
	Node **temp = x->children;
	x->children = added_child->children;
	added_child->children = temp;
	tree_attach(x, added_child);
}

void tree_attach(Node *parent, Node *child) {
	int position = child->his.val[0] - '0';
	child->depth = parent->depth + parent->his.len - 1;
	if (!parent->children[position])
		parent->children[position] = child;
}
