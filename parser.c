#include "parser.h"

// command literals
#define CMD_DEC "DECLARE"
#define CMD_ENE "ENERGY"
#define CMD_EQU "EQUAL"
#define CMD_REM "REMOVE"
#define CMD_VAL "VALID"
// maximum energy, the literal representing UINT64_MAX in decimal
#define MAX_ENERGY "18446744073709551615"
// maximum line length
#define LINE_MAX_LEN 99999

//todo handle allocation errors in parser

static Node *root;
static char buffer[LINE_MAX_LEN];

// auxiliary function declarations

enum Command parse_command(const char *);
int command_match(const char *, const char *, size_t);
int is_history_digit(char);
void print_error();
size_t command_size(enum Command);
size_t energy_length(const char *);
size_t history_length(const char *);
size_t read_line();
void execute(const char *, enum Command, size_t, size_t);
void parse_line(const char *);

// auxiliary function definitions

enum Command parse_command(const char *str) {
	if (command_match(str, CMD_DEC, sizeof(CMD_DEC))) return DECLARE;
	if (command_match(str, CMD_ENE, sizeof(CMD_ENE))) return ENERGY;
	if (command_match(str, CMD_EQU, sizeof(CMD_EQU))) return EQUAL;
	if (command_match(str, CMD_REM, sizeof(CMD_REM))) return REMOVE;
	if (command_match(str, CMD_VAL, sizeof(CMD_VAL))) return VALID;
	return ERROR;
}

int command_match(const char *str, const char *cmd, size_t size) {
	return !strncmp(str, cmd, size - 1);
}

int is_history_digit(char c) {
	return c >= '0' && c < '4';
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


size_t read_line() {
	size_t ans;
	for (ans = 1; ans < LINE_MAX_LEN; ++ans) {
		buffer[ans - 1] = ' ';
		buffer[ans] = '\0';
		int c = getchar();
		if (c == EOF) return 0;
		if (c == '\n') return ans;
		buffer[ans - 1] = (char) c;
	}
	return ans;
}

void print_error() {
	fprintf(stdout, "ERROR\n");
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
			if (s2) {
				Node *t = tree_find_split(root, arg1);
				if (!t) return;
				if (!strcmp(arg2, MAX_ENERGY)) {
					add_energy(t, energy_convert(UINT64_MAX));
				} else {
					uint64_t a2 = strtoull(arg2, NULL, 10);
					if (a2 == 0 || a2 >= UINT64_MAX) return print_error();
					add_energy(t, energy_convert(a2));
				}
			} else {
				const Node *t = tree_find_exact(root, arg1);
				energy_print(get_energy(t));
			}
			break;
		}
		case (EQUAL) : {
			if (!s2) return print_error();
			const Node *t1 = tree_find_exact(root, arg1);
			const Node *t2 = tree_find_exact(root, arg2);
			if (!t1 || !t2) return print_error();
			Energy *e1 = get_energy(t1), *e2 = get_energy(t2);
			if (!e1 || !e2) {
				return print_error();
			}
			set_union(e1, e2);
		}
		case (REMOVE) : {
			tree_remove(root, arg1);
		}
		case (VALID) : {
			if (tree_find_exact(root, arg1)) {
				puts("YES");
			} else {
				puts("NO");
			}
		}
		default: break;
	}
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

// linked functions

void initialize() {
	root = tree_init();
}

void read_input() {
	for (;;) {
		size_t length = read_line();
		// EOF
		if (length == 0) return;
		// empty line
		if (length == 1) continue;
		if (buffer[0] != '#')
			parse_line(buffer);
	}
}
