#include "energy.h"

struct Energy {
	energy_t val;
	size_t size;
	Energy *parent;
	Energy *left, *right;
	Node *node;
};

// Auxiliary functions

void energy_attach(Energy *, Energy *);
void energy_join(Energy *e1, Energy *e2);

energy_t avg(energy_t e1, energy_t e2) {
	if (e1 > e2) return avg(e2, e1);
	energy_t d = e2 - e1;
	return e1 + d / 2;
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

Energy * energy_init(energy_t val) {
	Energy *ans = (Energy *) malloc(sizeof(Energy));
	if (ans) {
		ans->val = val;
		ans->parent = ans;
		ans->size = 1;
		ans->left = ans->right = ans;
	}
	return ans;
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
	Energy *root = energy_find(x);
	if (x == root && x != x->left) {
		Energy *neighbor = x->left;
		*get_energy(neighbor->node) = x;
		--x->size;
		x = neighbor;
	}
	energy_join(x->left, x->right);
	free(x);
}

void energy_print(const Energy *x) {
	if (x) fprintf(stdout, "%llu\n", x->val);
}
