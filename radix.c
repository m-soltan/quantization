#include "radix.h"

/*
 * the size of the alphabet - the 4 digits 0, 1, 2, 3 plus 1 for the 5th
 * place, reserved for a marker
 */
#define DEGREE 4

typedef struct List List;

struct List {
	List *next;
	Node **val;
};

struct Node {
	size_t depth;
	History his;
	Energy *energy;
	Node **children;
};

// auxiliary functions

int node_init_fields(Node *x, const char *s) {
	x->children = (Node **) calloc(DEGREE, sizeof(Node *));
	if (x->children) {
		x->depth = 0;
		hist_init(&x->his, s);
		return 0;
	}
	return 1;
}

List * list_init() {
	List *ans = (List *) malloc(sizeof(List));
	if (ans) {
		ans->next = NULL;
		ans->val = NULL;
	}
	return ans;
}

Node * node_init(const char *str) {
	Node *ans = (Node *) malloc(sizeof(Node));
	if (ans) {
		if (!node_init_fields(ans, str))
			return ans;
		free(ans);
	}
	return NULL;
}

Node ** list_pop(List **x) {
	assert(*x);
	Node **ans = (*x)->val;
	List *temp = *x;
	*x = (*x)->next;
	free(temp);
	return ans;
}

int list_push(List **list, Node **element) {
	List *head = (List *) malloc(sizeof(List));
	if (!head)
		return 1;
	head->next = *list;
	head->val = element;
	*list = head;
	return 0;
}

void node_destroy(Node **x, List **l) {
	for (size_t i = 0; i < DEGREE; ++i) {
		Node **child = &(*x)->children[i];
		if (child)
			list_push(l, child);
	}
	*x = NULL;
}

int node_fit(const Node *x, const char *pattern) {
	return (!strncmp(x->his.val, pattern, x->his.len - 1));
}

void node_absorb(Node *x, size_t child_num) {
	assert(debug_single_child(x));
	assert(child_num < DEGREE);
	Node *child = x->children[child_num];
	History temp = hist_concat(&x->his, &child->his);
	hist_destroy(&x->his);
	hist_destroy(&child->his);
	x->his = temp;
	free(x->children);
	x->children = child->children;
}

// auxiliary function used to attach a child to a specified parent
void node_attach(Node *parent, Node *child) {
	int position = child->his.val[0] - '0';
	child->depth = parent->depth + parent->his.len - 1;
	if (!parent->children[position])
		parent->children[position] = child;
}

void node_split(Node *x, size_t prefix_size) {
	History old = x->his;
	Node *added_child = node_init(NULL);
	x->his = hist_prefix(&old, prefix_size);
	added_child->his = hist_suffix(&old, old.len - prefix_size + 1);
	hist_destroy(&old);
	Node **temp = x->children;
	x->children = added_child->children;
	added_child->children = temp;
	node_attach(x, added_child);
}

// linked functions

Energy ** get_energy(Node *x) {
	return &x->energy;
}

Node ** get_child(const Node *parent, const char *pattern) {
	assert(pattern[0] >= '0' && pattern[0] < '4');
	return &parent->children[pattern[0] - '0'];
}

Node * tree_find(Node *tree, const char *str) {
	Node *next = *get_child(tree, str), *previous = tree;
	while (next && node_fit(next, str)) {
		str += tree->his.len - 1;
		if (str[0] == '\0')
			break;
		previous = tree;
		tree = next;
		next = *get_child(tree, str);
	}
	return previous;
}

Node * tree_find_exact(Node *tree, const char *str) {
	Node *parent = tree_find(tree, str);
	str += parent->depth + parent->his.len - 1;
	size_t len = strlen(str);
	Node *child = *get_child(parent, str);
	if (child && !strncmp(child->his.val, str, len))
		return child;
	return NULL;
}

Node * tree_find_split(Node *tree, const char *str) {
	Node *child, *parent = tree_find(tree, str);
	str += parent->depth;
	size_t len = strlen(str);
	child = *get_child(parent, str);
	if (!child || strncmp(child->his.val, str, len - 1) != 0)
		return NULL;
	if (child->his.len == len)
		return child;
	node_split(child, 1 + len);
	return child;
}

Node * tree_init() {
	return node_init("");
}

void add_energy(Node *x, energy_t e) {
	x->energy = energy_init(e);
}

void tree_destroy(Node **x) {
	List *l = list_init();
	node_destroy(x, &l);
	while (l->next) {
		Node **i = list_pop(&l);
		node_destroy(i, &l);
	}
}

void tree_destroy_rec(Node **x) {
	if (!*x)
		return;
	for (size_t i = 0; i < DEGREE; ++i) {
		tree_destroy_rec((*x)->children + i);
	}
	free((*x)->children);
	energy_destroy((*x)->energy);
	hist_destroy(&(*x)->his);
	free(*x);
	*x = NULL;
}

void tree_insert(Node *tree, const char *str) {
	Node *new_child, *old_child, *parent;
	tree = tree_find(tree, str);
	str += tree->depth + tree->his.len - 1;
	old_child = *get_child(tree, str);
	parent = tree;
	if (old_child) {
		if (!strncmp(str, old_child->his.val, strlen(str))) return;
		node_split(old_child, 1 + first_difference(&old_child->his, str));
		parent = old_child;
		str += old_child->his.len - 1;
	} else {
		str += tree->his.len - 1;
	}
	new_child = node_init(str);
	node_attach(parent, new_child);
}

void tree_remove(Node *tree, const char *str) {
	tree_insert(tree, str);
	Node **child, *parent = tree_find(tree, str);
	str += parent->depth;
	child = get_child(parent, str);
	if (node_fit(*child, str))
		return tree_destroy_rec(child);
	assert(0);
}

// debug functions

int debug_single_child(const Node *x) {
	int found_one = 0;
	for (size_t i = 0; i < DEGREE; ++i) {
		if (x->children[i]) {
			if (found_one)
				return 0;
			found_one = 1;
		}
	}
	return 1;
}