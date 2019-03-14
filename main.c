#include <stdio.h>
//#include "history.h"
#include "radix.h"


//todo test splitting at a lower level
void test1();
void test2();
void test3();
void test4();
void test5();
void test6();
void test7();
void test8();
void test9();

int main() {
//	size_t a = hist_match(h1, "hello world");
//	test1();
//	test2();
//	test3();
//	test4();
//	test5();
//	test6();
//	test7();
//	test8();
	test9();
//	hist_destroy(h1);
	return 0;
}

void test1() {
	History h1, h2;
	hist_init(&h1, "123");
	hist_init(&h2, "01201");
	hist_destroy(&h1);
	hist_destroy(&h2);
}

void test2() {
	Node *t1 = node_init("012012");
	Node *t2 = node_init("000");
	tree_attach(t1, t2);
	Node *f = tree_find(t1, "01200");
}

void test3() {
	History h1, h2;
	hist_init(&h1, "012");
	hist_init(&h2, "3210");
	History h3 = hist_concat(&h1, &h2);
	hist_destroy(&h1);
	hist_destroy(&h2);
	hist_destroy(&h3);
}

void test4() {
	Node *t1 = tree_init();
	tree_insert(t1, "10211010");
	node_split(t1, 4);
}

void test5() {
	Node *t1 = tree_init();
	tree_insert(t1, "1212");
	tree_insert(t1, "121212");
}

void test6() {
	Node *t1 = tree_init();
	tree_insert(t1, "1212");
	tree_insert(t1, "1200");
}

void test7() {
	Node *t1 = tree_init(), *t2;
	tree_insert(t1, "1010");
	tree_insert(t1, "101010");
	t2 = tree_find(t1, "1010");
	node_absorb(t2, 1);
}

void test8() {
	Node *t1 = tree_init(), *t2;
	tree_insert(t1, "120201");
	t2 = tree_find(t1, "120201");
}

void test9() {
	Node *t1 = tree_init();
	tree_insert(t1, "120120");
	tree_insert(t1, "120002");
	tree_insert(t1, "12010121");
//	tree_insert(t1, "10011");
	tree_destroy_rec(&t1);
}