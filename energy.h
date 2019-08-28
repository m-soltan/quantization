#ifndef QUANTIZATION_ENERGY_H
#define QUANTIZATION_ENERGY_H

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef uint64_t energy_t;
typedef struct Energy Energy;

uint64_t energy_value(const Energy *);
Energy * energy_find(Energy *);
Energy * energy_init(energy_t);
Energy * energy_mod(Energy *, energy_t);
int energy_print(Energy *);
int energy_start(void);
void energy_destroy(Energy *);
void energy_finish(void);
void energy_union(Energy *x, Energy *y);

#endif //QUANTIZATION_ENERGY_H
