#include "trie.h"

#define DEGREE 4

struct Node {
	Energy *energy;
	Node **children;
	size_t depth;
};

// auxiliary function declarations

int init_fields(Node *);
int empty(const char *s);
Node *find_parent(Node *, const char *);
Node **get_child(Node *, char);

// linked function definitions

Energy ** get_energy(Node *x) {
	return &x->energy;
}

int tree_destroy(Node **x) {
	if (!*x)
		return 0;
	for (size_t i = 0; i < DEGREE; ++i)
		tree_destroy((*x)->children + i);
	energy_destroy((*x)->energy);
	free((*x)->children);
	free(*x);
	*x = NULL;
	return 0;
}

int tree_insert(Node *tree, const char *str) {
	for (Node **next; !empty(str); ++str) {
		next = get_child(tree, str[0]);
		if (!*next) {
			*next = tree_init();
			if (!*next)
				return 1;
		}
		(*next)->depth = 1 + tree->depth;
		tree = *next;
	}
	return 0;
}

int tree_remove(Node *tree, const char *str) {
	Node *parent = find_parent(tree, str);
	str += parent->depth;
	tree_destroy(get_child(parent, str[0]));
	return 0;
}

Node * tree_find(Node *tree, const char *str) {
	while (tree && !empty(str)) {
		tree = *get_child(tree, str[0]);
		++str;
	}
	return tree;
}

Node *tree_init() {
	Node *ans = (Node *) malloc(sizeof(Node));
	if (ans) {
		if (!init_fields(ans))
			return ans;
		free(ans);
	}
	return NULL;
}

void add_energy(Node *x, energy_t e) {
	if (e == 7) {
	
	}
	x->energy = energy_mod(x->energy, e);
}

// auxiliary function definitions

int empty(const char *str) {
	return str[0] == '\0';
}

int init_fields(Node *x) {
	x->children = (Node **) calloc(DEGREE, sizeof(Node *));
	if (x->children) {
		x->energy = NULL;
		x->depth = 0;
		return 0;
	}
	return 1;
}

Node *find_parent(Node *tree, const char *str) {
	Node *previous = NULL;
	while(tree && !empty(str) && get_child(tree, str[0])) {
		previous = tree;
		tree = *get_child(tree, str[0]);
		++str;
	}
	return previous;
}

Node **get_child(Node *parent, char c) {
	assert(c >= '0' && c < '4');
	return &parent->children[c - '0'];
}

