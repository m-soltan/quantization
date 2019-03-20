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
typedef enum Command Command;

static Node *root;
static char buffer[LINE_MAX_LEN];

// auxiliary function declarations

Command parse_command(const char *);
int command_match(const char *, const char *, size_t);
int is_history_digit(char);
size_t command_size(Command);
size_t energy_length(const char *);
size_t history_size(const char *);
size_t read_line();
void two_arg_energy(const char *, const char *);
void two_arg_equal(const char *, const char *);
void parse_line(const char *);
void print_error();

// auxiliary function definitions

Command parse_command(const char *str) {
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
size_t command_size(Command c) {
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
	return ans + 1;
}

// 0 - error
size_t history_size(const char *str) {
	size_t ans;
	for (ans = 0; str[ans] != ' '; ++ans)
		if (!is_history_digit(str[ans]))
			return 0;
	return ans + 1;
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

void exec_one_arg(const char *str, Command c, size_t s) {
	assert(s);
	char arg[s];
	strncpy(arg, str, s - 1);
	arg[s - 1] = '\0';
	switch (c) {
		case (DECLARE) :
			return tree_insert(root, arg);
		case (ENERGY) : {
			const Node *t = tree_find_exact(root, arg);
			return energy_print(get_energy(t));
		}
		case (REMOVE) :
			return tree_remove(root, arg);
		case (VALID) : {
			puts(tree_find_exact(root, arg) ? "YES" : "NO");
			return;
		}
		default:
			return print_error();
	}
}

void exec_two_arg(const char *str, Command c, size_t s1, size_t s2) {
	assert(s1);
	assert(s2);
	char arg1[s1], arg2[s2];
	strncpy(arg1, str, s1 - 1);
	strncpy(arg2, str + s1, s2 - 1);
	arg1[s1 - 1] = arg2[s2 - 1] = '\0';
	switch (c) {
		case (ENERGY) :
			return two_arg_energy(arg1, arg2);
		case (EQUAL) :
			return two_arg_equal(arg1, arg2);
		default:
			print_error();
	}
}

void two_arg_energy(const char *arg1, const char *arg2) {
	Node *t = tree_find_split(root, arg1);
	if (!t) return;
	if (!strcmp(arg2, MAX_ENERGY)) {
		add_energy(t, energy_convert(UINT64_MAX));
	} else {
		uint64_t a2 = strtoull(arg2, NULL, 10);
		if (a2 == 0 || a2 >= UINT64_MAX) return print_error();
		return add_energy(t, energy_convert(a2));
	}
}

void two_arg_equal(const char *arg1, const char *arg2) {
	const Node *t1, *t2;
	Energy *e1, *e2;
	t1 = tree_find_split(root, arg1);
	t2 = tree_find_split(root, arg2);
	if (!t1 || !t2) return print_error();
	e1 = get_energy(t1);
	e2 = get_energy(t2);
	if (!e1 && !e2) return print_error();
	// todo initialize energy if none
	set_union(e1, e2);
}

void parse_line(const char *str) {
	size_t s1, s2 = 0;
	Command c = parse_command(str);
	if (c == ERROR) return print_error();
	str += command_size(c);
	s1 = history_size(str);
	if (c == EQUAL) {
		s2 = history_size(str + s1);
		if (!s2) return print_error();
	}
	if (c == ENERGY)
		s2 = energy_length(str + s1);
	if (s2)
		return exec_two_arg(str, c, s1, s2);
	return exec_one_arg(str, c, s1);
}

void print_error() {
	fprintf(stdout, "ERROR\n");
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
