#include "parser.h"

// maximum command size
#define COMMAND_MAXSIZE 7
// maximum line length
#define LINE_MAXLEN 999999

enum Command {
	DECLARE,
	ENERGY,
	ERROR,
	EQUAL,
	REMOVE,
	VALID
};

int clear_line(const int ans) {
	for (int c = fgetc(stdin); c != '\n' && c != EOF; c = fgetc(stdin));
	return ans;
}

enum Command parse_command(const char *str) {
	if (!strcmp(str, "DECLARE")) return DECLARE;
	if (!strcmp(str, "ENERGY")) return ENERGY;
	if (!strcmp(str, "EQUAL")) return EQUAL;
	if (!strcmp(str, "REMOVE")) return REMOVE;
	if (!strcmp(str, "VALID")) return VALID;
	return ERROR;
}

int read_command(int first) {
	int *buffer = (int *) calloc(COMMAND_MAXSIZE + 1, sizeof(int));
	for (size_t i = 0; i < 1 + COMMAND_MAXSIZE; ++i) {
		int c = fgetc(stdin);
		if (!isalpha(c))
			return clear_line(-1);
	}
	
	free(buffer);
}

// -1 - end of file
// 0 - error
// 1 - empty or comment
// 2 - end of file
int read_line(const char *line) {
	char *command_name = malloc(sizeof(char) * (1 + COMMAND_MAXSIZE));
	
	if (line[0] == '#') return clear_line(1);
	if (line[0] == '\n') return 1;
	if (line[0] == EOF) return -1;
	for (size_t i = 0; line[i]; ++i) {
	
	}
}

void read_input() {
}