#include <stdio.h>
#include "history.h"
#include "radix.h"

int main() {
	History *h1 = hist_init("123");
	History *h2 = hist_init("hello");
//	size_t a = hist_match(h1, "hello world");
	Node *t1 = tree_init("012012");
	Node *t2 = tree_init("000");
	tree_attach(t1, t2);
	Node *f = tree_find(t1, "0120121");
//	hist_destroy(h1);
	return 0;
}