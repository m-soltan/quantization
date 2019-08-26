#include "trie.h"

#define DEGREE 4

typedef struct List List;

struct Node {
	Energy *energy;
	Node **children;
	size_t depth;
};

struct List {
	Node **v;
	size_t length, max_length;
};

static const size_t start_length = 16;
static List list;

int init_fields(Node *);
int empty(const char *s);
static int list_empty(void);
static Node *list_pop(void);
static int list_push(Node *);
static int destroy(Node **pNode);
Node *find_parent(Node *, const char *);
Node **get_child(Node *, char);

// linked function definitions

Energy ** get_energy(Node *x) {
	return &x->energy;
}

int tree_destroy(Node **x) {
	assert(x != NULL);
	list_push(*x);
	*x = NULL;
	while (!list_empty()) {
		Node *node = list_pop();
		destroy(&node);
	}
	return 0;
}

int tree_insert(Node *tree, const char *str) {
	assert(str);
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
	assert(parent);
	assert(parent != tree);
	str += parent->depth;
	tree_destroy(get_child(parent, str[0]));
	return 0;
}

Node * tree_find(Node *tree, const char *str) {
	while (tree && tree->children && !empty(str)) {
		tree = *get_child(tree, str[0]);
		++str;
	}
	if (tree == NULL) {
		return NULL;
	} else {
		if (str[0] == '\0')
			return tree;
		else
			return NULL;
	}
}

Node *tree_init() {
	list = (List) {
		.v = calloc(start_length, sizeof(Node *)),
		.max_length = start_length,
		.length = 0
	};
	if (list.v) {
		Node *ans = calloc(1, sizeof(Node));
		if (ans) {
			if (!init_fields(ans))
				return ans;
			free(ans);
		}
		free(list.v);
	}
	return NULL;
}

void add_energy(Node *x, energy_t e) {
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

int list_empty(void) {
	return list.length == 0;
}

static Node *list_pop(void) {
	assert(!list_empty());
	--list.length;
	return list.v[list.length];
}

int list_push(Node *node) {
	if (list.length == list.max_length) {
		list.max_length *= 4;
		Node **temp = realloc(list.v, list.max_length);
		if (temp == NULL)
			return 1;
	}
	assert(list.max_length > list.length);
	list.v[list.length] = node;
	++list.length;
	return 0;
}

int destroy(Node **pNode) {
	Node *node = *pNode;
	*pNode = NULL;
	energy_destroy(node->energy);
	if (node->children == NULL)
		return 0;
	for (size_t i = 0; i < DEGREE; ++i) {
		Node *child = node->children[i];
		if (child) {
			int error = list_push(child);
			assert(error == 0);
		}
	}
	free(node->children);
	return 0;
}
