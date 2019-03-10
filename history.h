#include <assert.h>
#include <stdlib.h>
#include <string.h>

#ifndef QUANTIZATION_HISTORY_H
#define QUANTIZATION_HISTORY_H

typedef struct History {
	size_t len;
	char * val;
} History;

History hist_concat(const History *, const History *);
History hist_prefix(const History *, size_t);
History hist_suffix(const History *, size_t);
size_t hist_match(const History *, const char *);
void hist_destroy(History);
void hist_init(History *, const char *);

#endif //QUANTIZATION_HISTORY_H
