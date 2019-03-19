#include "energy.h"

struct Energy {
	energy_t val;
	size_t size;
	Energy *parent;
};

// Auxiliary functions

energy_t avg(energy_t e1, energy_t e2) {
	if (e1 > e2) return avg(e2, e1);
	energy_t d = e2 - e1;
	return e1 + d / 2;
}

// a Disjoint-Set implementation with path-splitting and union by size

Energy * find(Energy *x) {
	for (Energy *next; x->parent != x; x = next) {
		next = x->parent;
		x->parent = next->parent;
	}
	return x;
}

energy_t energy_convert(uint64_t x) {
	return x;
}

Energy *make_set(energy_t val) {
	Energy *ans = (Energy *) malloc(sizeof(Energy));
	if (ans) {
		ans->val = val;
		ans->parent = ans;
		ans->size = 1;
	}
	return ans;
}


void set_union(Energy *x, Energy *y) {
	Energy *x_root = find(x), *y_root = find(y);
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
}

//todo
void energy_destroy(Energy *x) {
	(void) x;
}

void energy_print(const Energy *x) {
	if (x) fprintf(stdout, "%llu\n", x->val);
}
