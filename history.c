#include "history.h"
#include <stdbool.h>
#include <stdio.h>

History hist_init(const char *str) {
	History ans;
	char **val_tmp = (char **) &ans.val;
	size_t *len_tmp = (size_t *) &ans.len;
	if (str)
		*len_tmp = 1 + strlen(str);
	*val_tmp = str ? malloc(ans.len * sizeof(char)) : NULL;
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