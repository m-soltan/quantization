#include <stdlib.h>
#include <string.h>

#ifndef QUANTIZATION_HISTORY_H
#define QUANTIZATION_HISTORY_H

typedef struct History {
	const size_t len;
	const char *const val;
} History;

History hist_concat(const History *, const History *);
History hist_init(const char *);
History hist_prefix(const History *, size_t);
History hist_suffix(const History *, size_t);
size_t hist_match(const History *, const char *);
void hist_destroy(History *);

#endif //QUANTIZATION_HISTORY_H
