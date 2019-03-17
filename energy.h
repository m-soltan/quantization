#ifndef QUANTIZATION_ENERGY_H
#define QUANTIZATION_ENERGY_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef uint64_t energy_t;
typedef struct Energy Energy;

void energy_destroy(Energy *);
void energy_print(const Energy *);

#endif //QUANTIZATION_ENERGY_H
