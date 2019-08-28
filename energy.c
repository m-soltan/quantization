#include "energy.h"

typedef struct Energy_list Energy_list;

struct Energy {
	energy_t val;
	size_t size;
	Energy *parent;
	Energy *left, *right;
};

struct Energy_list {
	Energy **v;
	size_t length, max_length;
};

static Energy_list list;

static energy_t avg(energy_t, energy_t);
static int reserve(void);
static void energy_attach(Energy *, Energy *);
static void energy_join(Energy *e1, Energy *e2);
static Energy *init(void);

// a Disjoint-Set implementation with path-splitting and union by size

Energy * energy_find(Energy *x) {
	for (Energy *next; x->parent != x; x = next) {
		next = x->parent;
		x->parent = next->parent;
	}
	return x;
}

energy_t energy_value(const Energy *x) {
	assert(x);
	return x->val;
}

Energy *energy_init(energy_t val) {
	Energy *ans = init();
	if (ans) {
		ans->val = val;
		ans->parent = ans;
		ans->size = 1;
		ans->left = ans->right = ans;
	}
	return ans;
}

Energy *energy_mod(Energy *old, energy_t new_val) {
	if (old) {
		Energy *root = energy_find(old);
		root->val = new_val;
		return old;
	} else {
		Energy *ans = energy_init(new_val);
		return ans;
	}
}

int energy_print(Energy *x) {
	if (!x) {
		return 1;
	} else {
		fprintf(stdout, "%" PRIu64 "\n", energy_find(x)->val);
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

void energy_finish(void) {
	for (size_t i = 0; i < list.length; ++i)
		free(list.v[i]);
	free(list.v);
}

int energy_start(void) {
	const size_t init_max = 16;
	list = (Energy_list) {
		.v = calloc(init_max, sizeof(Energy)),
		.length = 0,
		.max_length = init_max
	};
	if (list.v)
		return 0;
	else
		return -1;
}

static energy_t avg(energy_t e1, energy_t e2) {
	if (e1 > e2) return avg(e2, e1);
	energy_t d = e2 - e1;
	return e1 + d / 2;
}

static void energy_attach(Energy *e1, Energy *e2) {
	energy_join(e2->left, e1->right);
	energy_join(e1, e2);
}

static void energy_join(Energy *e1, Energy *e2) {
	e1->right = e2;
	e2->left = e1;
}

int reserve(void) {
	if (list.length == list.max_length) {
		const size_t new_max = 4 * list.max_length;
		assert(new_max);
		Energy **temp = realloc(list.v, new_max * sizeof(Energy));
		if (temp) {
			list.v = temp;
			list.max_length = new_max;
		} else {
			return -1;
		}
	}
	return 0;
}

static Energy *init(void) {
	int error = reserve();
	if (error == 0) {
		Energy *ans = calloc(1, sizeof(Energy));
		if (ans) {
			list.v[list.length] = ans;
			++list.length;
			return ans;
		}
	}
	return NULL;
}
