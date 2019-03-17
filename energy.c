#include "energy.h"

struct Energy {
	energy_t val;
	size_t size;
	Energy *parent;
};

/*
 * Auxiliary functions
 * a Disjoint-Set implementation with path-splitting and union by size
 */

Energy * find(Energy *x) {
	for (Energy *next; x->parent != x; x = next) {
		next = x->parent;
		x->parent = next->parent;
	}
	return x;
}

Energy * make_set(energy_t val) {
	Energy *ans = (Energy *) malloc(sizeof(Energy));
	if (ans) {
		ans->val = val;
		ans->parent = ans;
		ans->size = 1;
	}
	return ans;
}

void set_union(Energy *x, Energy *y) {
	Energy *xRoot = find(x), *yRoot = find(y);
	if (xRoot == yRoot)
		return;
	if (xRoot->size < yRoot->size) {
		Energy *temp = xRoot;
		xRoot = yRoot;
		yRoot = temp;
	}
	yRoot->parent = xRoot;
	xRoot->size += yRoot->size;
}

//todo
void energy_destroy(Energy *x) {
	(void) x;
}

void energy_print(const Energy *x) {
	fprintf(stdout, "%llu\n", x->val);
}
