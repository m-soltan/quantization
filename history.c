#include "history.h"
#include <stdbool.h>
#include <stdio.h>

History hist_concat(const History *left, const History *right) {
	History ans;
	hist_init(&ans, NULL);
	char *str = malloc(sizeof(char) * (left->len + right->len - 1));
	if (str) {
		strcpy(str, left->val);
		strcpy(str + left->len - 1, right->val);
		hist_init(&ans, str);
		free(str);
	}
	return ans;
}

void hist_init(History *x, const char *str) {
	if (!str)
		return;
	x->len = 1 + strlen(str);
	x->val = malloc(x->len * sizeof(char));
	if (x->val)
		strncpy(x->val, str, x->len);
}

History hist_prefix(const History *source, size_t size) {
	History ans;
	hist_init(&ans, NULL);
	if (size <= source->len) {
		char *prefix = malloc(size * sizeof(char));
		if (prefix) {
			strncpy(prefix, source->val, size - 1);
			hist_init(&ans, prefix);
			free(prefix);
		}
	}
	return ans;
}

History hist_suffix(const History * source, size_t size) {
	History ans;
	hist_init(&ans, NULL);
	if (size <= source->len) {
		char *suffix = (char *) malloc(size * sizeof(char));
		if (suffix) {
			strncpy(suffix, source->val + source->len - size, size - 1);
			hist_init(&ans, suffix);
			free(suffix);
		}
	}
	return ans;
}

size_t hist_match(const History *text, const char *pattern) {
	for (const char *p = pattern; true; ++p)
		if (!*p || *p != text->val[p - pattern])
			return p - pattern;
}

void hist_destroy(History x) {
	free(x.val);
}