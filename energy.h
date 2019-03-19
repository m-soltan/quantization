#ifndef QUANTIZATION_ENERGY_H
#define QUANTIZATION_ENERGY_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef uint64_t energy_t;
typedef struct Energy Energy;

energy_t energy_convert(uint64_t x);
Energy * make_set(energy_t);
void energy_destroy(Energy *);
void energy_print(const Energy *);
void set_union(Energy *x, Energy *y);

#endif //QUANTIZATION_ENERGY_H
