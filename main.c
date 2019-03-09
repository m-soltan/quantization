#include <stdio.h>
#include "history.h"
#include "radix.h"

void test1();
void test2();

int main() {
	History h2 = hist_init("");
//	size_t a = hist_match(h1, "hello world");
	Node *t1 = tree_init("012012");
	Node *t2 = tree_init("000");
	tree_attach(t1, t2);
	Node *f = tree_find(t1, "01200");
//	hist_destroy(h1);
	return 0;
}

void test1() {
	History h1 = hist_init("123");
	History h2 = hist_init("01201");
}

void test2() {
	Node *t1 = tree_init("012012");
	Node *t2 = tree_init("000");
}