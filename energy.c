#include "energy.h"

typedef struct Energy_list Energy_list;

struct Energy {
	energy_t val;
	int is_removed;
	int pad;
	size_t removed_count;
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
static void clear_all(Energy *root);
static void clear_removed(Energy *old_root);
static void energy_init_fields(Energy *e, energy_t val, size_t size);
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
		ans->removed_count = 0;
		ans->is_removed = 0;
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
		assert(ans);
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

static void energy_init_fields(Energy *e, energy_t val, size_t size) {
	e->val = val;
	e->is_removed = 0;
	e->removed_count = 0;
	e->size = size;
	e->parent = e->left = e->right = e;
}

static void energy_attach(Energy *e1, Energy *e2) {
	energy_join(e2->left, e1->right);
	energy_join(e1, e2);
}

static void energy_join(Energy *e1, Energy *e2) {
	e1->right = e2;
	e2->left = e1;
}

static void clear_removed(Energy *old_root) {
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

static void clear_all(Energy *root) {
	Energy *last = root->left;
	for (Energy *i = root, *next; ; i = next) {
		next = i->right;
		free(i);
		if (i == last)
			break;
	}
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
			assert(0); // todo
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
	assert(0);
	return NULL;
}
