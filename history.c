#include "history.h"
#include <stdbool.h>
#include <stdio.h>

History hist_concat(const History *left, const History *right) {
	char *str = malloc(sizeof(char) * (left->len + right->len - 1));
	if (!str) return hist_init(NULL);
	strcpy(str, left->val);
	strcpy(str + left->len - 1, right->val);
	History ans = hist_init(str);
	free(str);
	return ans;
}

History hist_init(const char *str) {
	History ans;
	char **val_tmp = (char **) &ans.val;
	size_t *len_tmp = (size_t *) &ans.len;
	*val_tmp = NULL;
	if (str) {
		*len_tmp = 1 + strlen(str);
		*val_tmp = malloc(ans.len * sizeof(char));
	}
	if (ans.val)
		strncpy(*val_tmp, str, ans.len);
	return ans;
}

History hist_prefix(const History *source, size_t size) {
	if (size > source->len)
		return hist_init(NULL);
	char *prefix = malloc(size * sizeof(char));
	if (!prefix)
		return hist_init(NULL);
	strncpy(prefix, source->val, size - 1);
	History ans = hist_init(prefix);
	free(prefix);
	return ans;
}

History hist_suffix(const History * source, size_t size) {
	char *suffix;
	if (size > source->len)
		return hist_init(NULL);
	suffix = (char *) malloc(size * sizeof(char));
	if (suffix)
		strncpy(suffix, source->val + source->len - size, size - 1);
	
	History ans = hist_init(suffix);
	free(suffix);
	return ans;
}

size_t hist_match(const History *text, const char *pattern) {
	for (const char *p = pattern; true; ++p)
		if (!*p || *p != text->val[p - pattern])
			return p - pattern;
}

void hist_destroy(History *x) {
	free((void *) x->val);
	free(x);
}