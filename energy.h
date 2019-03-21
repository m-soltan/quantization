#ifndef QUANTIZATION_ENERGY_H
#define QUANTIZATION_ENERGY_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef uint64_t energy_t;
typedef struct Energy Energy;

energy_t energy_convert(uint64_t x);
Energy * energy_find(Energy *);
Energy * energy_init(energy_t val);
void energy_destroy(Energy *);
void energy_print(const Energy *);
void energy_union(Energy *x, Energy *y);

#endif //QUANTIZATION_ENERGY_H
