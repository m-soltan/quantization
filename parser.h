#ifndef QUANTIZATION_PARSER_H
#define QUANTIZATION_PARSER_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "radix.h"
#include "trie.h"

enum Command {
	DECLARE,
	ENERGY,
	ERROR,
	EQUAL,
	REMOVE,
	VALID
};

void initialize();
void read_input();

#endif //QUANTIZATION_PARSER_H
