#include "energy.h"

struct Energy {
	energy_t val;
	int is_removed;
	size_t removed_count;
	size_t size;
	Energy *parent;
	Energy *left, *right;
};

// Auxiliary functions

energy_t avg(energy_t, energy_t);
void energy_init_fields(Energy *e, energy_t val, size_t size);
void energy_attach(Energy *, Energy *);
void energy_join(Energy *e1, Energy *e2);

energy_t avg(energy_t e1, energy_t e2) {
	if (e1 > e2) return avg(e2, e1);
	energy_t d = e2 - e1;
	return e1 + d / 2;
}

void energy_init_fields(Energy *e, energy_t val, size_t size) {
	e->val = val;
	e->is_removed = 0;
	e->removed_count = 0;
	e->size = size;
	e->parent = e->left = e->right = e;
}

void clear_all(Energy *root) {
	for (Energy *i = root; ; i = i->right) {
		free(i);
		if (i == root->left)
			break;
	}
}

void clear_removed(Energy *old_root) {
	Energy *new_root = old_root;
	old_root->size -= old_root->removed_count;
	while (new_root->is_removed) new_root = new_root->right;
	energy_init_fields(new_root, old_root->val, old_root->size);
	Energy *prev = new_root;
	for (Energy *i = new_root->right; ; i = i->right) {
		if (i->is_removed) {
			free(i);
		} else {
			i->parent = new_root;
			i->left = prev;
			prev->right = i;
			prev = i;
		}
		if (i == new_root)
			break;
	}
}

void energy_attach(Energy *e1, Energy *e2) {
	energy_join(e2->left, e1->right);
	energy_join(e1, e2);
}

void energy_join(Energy *e1, Energy *e2) {
	e1->right = e2;
	e2->left = e1;
}

// a Disjoint-Set implementation with path-splitting and union by size

Energy * energy_find(Energy *x) {
	for (Energy *next; x->parent != x; x = next) {
		next = x->parent;
		x->parent = next->parent;
	}
	return x;
}

energy_t energy_convert(uint64_t x) {
	return x;
}

energy_t energy_value(const Energy *x) {
	assert(x);
	return x->val;
}

Energy * energy_init(energy_t val) {
	Energy *ans = (Energy *) malloc(sizeof(Energy));
	if (ans) {
		ans->val = val;
		ans->parent = ans;
		ans->size = 1;
		ans->removed_count = 0;
		ans->is_removed = 0;
		ans->left = ans->right = ans;
	}
	return ans;
}

Energy * energy_mod(Energy *old, energy_t new_val) {
	if (old) {
		Energy *root = energy_find(old);
		root->val = new_val;
		return old;
	} else {
		return energy_init(new_val);
	}
}

int energy_print(Energy *x) {
	if (!x) {
		return 1;
	} else {
		fprintf(stdout, "%llu\n", energy_find(x)->val);
		return 0;
	}
}

void energy_union(Energy *x, Energy *y) {
	Energy *x_root = energy_find(x), *y_root = energy_find(y);
	energy_t root_e = avg(x_root->val, y_root->val);
	if (x_root == y_root)
		return;
	if (x_root->size < y_root->size) {
		Energy *temp = x_root;
		x_root = y_root;
		y_root = temp;
	}
	y_root->parent = x_root;
	x_root->size += y_root->size;
	x_root->val = root_e;
	energy_attach(x_root, y_root);
}

void energy_destroy(Energy *x) {
	if (!x) return;
	Energy *root = energy_find(x);
	++root->removed_count;
	if (root->size == root->removed_count) {
		clear_all(root);
		return;
	}
	if (root->size > 1000 && root->removed_count * 4 > root->size * 3) {
		clear_removed(root);
		return;
	}
}

