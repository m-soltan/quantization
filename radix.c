//todo remove
#include <stdio.h>

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

// todo: add energy pointer
struct Node {
	size_t depth;
	History his;
	Energy *energy;
	Node **children;
};

List * list_init() {
	List *ans = (List *) malloc(sizeof(List));
	if (ans) {
		ans->next = NULL;
		ans->val = NULL;
	}
	return ans;
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

//

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
	get_child_ref(parent, pattern);
	return parent->children[pattern[0] - '0'];
}

Node ** get_child_ref(const Node *parent, const char *pattern) {
	assert(pattern[0] >= '0' && pattern[0] < '4');
	return &parent->children[pattern[0] - '0'];
}

Node * node_init(const char *str) {
	Node *ans = (Node *) malloc(sizeof(Node));
	if (ans) {
		if (!node_init_fields(ans, str)) {
			for (size_t i = 0; i < DEGREE; ++i)
				ans->children[i] = NULL;
			return ans;
		}
		free(ans);
	}
	return NULL;
}
// todo rewrite find_parent and define find using find_parent
Node * tree_find(const Node *tree, const char *str) {
	const Node *next = get_child(tree, str);
	while (next && node_fit(next, str)) {
		str += tree->his.len - 1;
		if (str[0] == '\0')
			break;
		tree = next;
		next = get_child(tree, str);
	}
	return (Node *) tree;
}

Node * tree_find_parent(const Node *tree, const char *str) {
	const Node *next = get_child(tree, str), *previous = tree;
	while (next && node_fit(next, str)) {
		str += tree->his.len - 1;
		if (str[0] == '\0')
			break;
		previous = tree;
		tree = next;
		next = get_child(tree, str);
	}
	return (Node *) previous;
}

Node * tree_init() {
	return node_init("");
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


void node_split(Node *x, size_t prefix_size) {
	History old = x->his;
	Node *added_child = node_init(NULL);
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
	energy_destroy((*x)->energy);
	fprintf(stdout, "%s\n", (*x)->his.val);
	hist_destroy(&(*x)->his);
	free(*x);
	*x = NULL;
}

void tree_insert(Node *tree, const char *str) {
	tree = tree_find_parent(tree, str);
	str += tree->depth + tree->his.len - 1;
	Node *old_child = get_child(tree, str), *parent = tree;
	if (old_child) {
		node_split(old_child, 1 + first_difference(&old_child->his, str));
		parent = old_child;
		str += old_child->his.len - 1;
	} else {
		str += tree->his.len - 1;
	}
	Node *new_child = node_init(str);
	tree_attach(parent, new_child);
}

void tree_remove(Node *tree, const char *str) {
	tree_insert(tree, str);
	Node **child, *parent = tree_find_parent(tree, str);
	str += parent->depth;
	child = get_child_ref(parent, str);
	if (node_fit(*child, str))
		return tree_destroy(child);
	assert(0);
}

// debug function definitions

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