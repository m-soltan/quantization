#include "parser.h"

int main() {
	int error_energy;
	initialize();
	error_energy = energy_start();
	if (error_energy == 0) {
		int error_input;
		error_input = read_input();
		energy_finish();
		finish();
		tree_finish();
		if (error_input == 0)
			return 0;
	}
	return 1;
}

