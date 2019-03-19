#include "parser.h"

// command literals
#define CMD_DEC "DECLARE"
#define CMD_ENE "ENERGY"
#define CMD_EQU "EQUAL"
#define CMD_REM "REMOVE"
#define CMD_VAL "VALID"
// maximum energy
#define MAX_ENERGY "18446744073709551615"
// maximum line length
#define LINE_MAX_LEN 99999

//todo handle allocation errors in parser

static Node *root;
static char buffer[LINE_MAX_LEN];

int is_history_digit(char);
void print_error();

// debug
void debug_print(const char *, size_t, size_t);

int is_history_digit(char c) {
	return c >= '0' && c < '4';
}

// 0 - error
size_t energy_length(const char *str) {
	size_t ans;
	for (ans = 0; str[ans] != ' '; ++ans)
		if (!isnumber(str[ans]) || ans >= sizeof(MAX_ENERGY))
			return 0;
	return ans;
}

// 0 - error
size_t history_length(const char *str) {
	size_t ans;
	for (ans = 0; str[ans] != ' '; ++ans)
		if (!is_history_digit(str[ans]))
			return 0;
	return ans;
}

// 0 - error
size_t command_size(enum Command c) {
	switch (c) {
		case (DECLARE) :
			return sizeof(CMD_DEC);
		case (ENERGY) :
			return sizeof(CMD_ENE);
		case (EQUAL) :
			return sizeof(CMD_EQU);
		case (REMOVE) :
			return sizeof(CMD_REM);
		case (VALID) :
			return sizeof(CMD_VAL);
		case (ERROR) : {
			assert(0);
			return 0;
		}
	}
}

int command_match(const char *str, const char *cmd, size_t size) {
	return !strncmp(str, cmd, size - 1);
}

enum Command parse_command(const char *str) {
	if (command_match(str, CMD_DEC, sizeof(CMD_DEC))) return DECLARE;
	if (command_match(str, CMD_ENE, sizeof(CMD_ENE))) return ENERGY;
	if (command_match(str, CMD_EQU, sizeof(CMD_EQU))) return EQUAL;
	if (command_match(str, CMD_REM, sizeof(CMD_REM))) return REMOVE;
	if (command_match(str, CMD_VAL, sizeof(CMD_VAL))) return VALID;
	return ERROR;
}

void execute(const char *str, enum Command c, size_t s1, size_t s2) {
	assert(s1);
	char arg1[s1], arg2[s2];
	strncpy(arg1, str, s1 - 1);
	arg1[s1 - 1] = '\0';
	if (s2) {
		strncpy(arg2, str + s1, s2 - 1);
		arg2[s2 - 1] = '\0';
	}
	switch (c) {
		case (DECLARE) : {
			tree_insert(root, arg1);
			break;
		}
		case (ENERGY) : {
			const Node *t = tree_find(root, arg1);
			break;
		}
		case (EQUAL) : {
		
		}
		case (REMOVE) : {
			tree_remove(root, arg1);
		}
		case (VALID) : {
		
		}
		default: break;
	}
}

void initialize() {
	root = tree_init();
}

void parse_line(const char *str) {
	size_t s1, s2 = 0;
	enum Command c = parse_command(str);
	if (c == ERROR) return print_error();
	str += command_size(c);
	s1 = history_length(str);
	if (c == EQUAL) {
		s2 = history_length(str + s1 + 1);
		if (!s2) return print_error();
	}
	if (c == ENERGY) s2 = energy_length(str + s1 + 1);
	return execute(str, c, s1, s2);
}

void print_error() {
	fprintf(stdout, "ERROR\n");
}

void read_input() {
	for (;;) {
		for (size_t i = 0; i < LINE_MAX_LEN - 1; ++i) {
			buffer[i] = ' ';
			buffer[i + 1] = '\0';
			int c = getchar();
			if (c == EOF) return;
			if (c == '\n') break;
			buffer[i] = (char) c;
		}
		switch (buffer[0]) {
			case ('\n') :
				break;
			case ('#') :
				break;
			default:
				parse_line(buffer);
		}
	}
}

// debug functions
void debug_print(const char *str, size_t s1, size_t s2) {
	printf("%s, %zu, %zu\n", str, s1, s2);
}