#include "history.h"
#include <stdbool.h>
#include <stdio.h>

History hist_concat(const History *left, const History *right) {
	History ans;
	hist_init(&ans, NULL);
	char *str = malloc(sizeof(char) * (left->size + right->size - 1));
	if (str) {
		strcpy(str, left->val);
		strcpy(str + left->size - 1, right->val);
		hist_init(&ans, str);
		free(str);
	}
	return ans;
}

void hist_init(History *x, const char *str) {
	if (!str)
		return;
	x->size = 1 + strlen(str);
	x->val = malloc(x->size * sizeof(char));
	if (x->val)
		strncpy(x->val, str, x->size);
}

History hist_prefix(const History *source, size_t size) {
	History ans;
	hist_init(&ans, NULL);
	if (size <= source->size) {
		char *prefix = malloc(size * sizeof(char));
		if (prefix) {
			strncpy(prefix, source->val, size - 1);
			prefix[size - 1] = '\0';
			hist_init(&ans, prefix);
			free(prefix);
		}
	}
	return ans;
}

History hist_suffix(const History * source, size_t size) {
	History ans;
	hist_init(&ans, NULL);
	if (size <= source->size) {
		char *suffix = (char *) malloc(size * sizeof(char));
		if (suffix) {
			strncpy(suffix, source->val + source->size - size, size - 1);
			suffix[size - 1] = '\0';
			hist_init(&ans, suffix);
			free(suffix);
		}
	}
	return ans;
}

size_t first_difference(const History *h, const char *str) {
	assert(h->val[0] == str[0]);
	size_t max_ans = h->size;
	for (size_t i = 1; str[i - 1] && i < max_ans; ++i)
		if (h->val[i] != str[i])
			return i;
	return max_ans;
}

void hist_destroy(History *x) {
	free(x->val);
}
