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
#define BASE_BUFFER_SIZE 140000

//todo handle allocation errors in parser
typedef enum Command Command;

static size_t buffer_size = BASE_BUFFER_SIZE;
static Node *root;
static char static_buffer[BASE_BUFFER_SIZE];
static char *buffer = static_buffer;

// auxiliary function declarations

Command parse_command(const char *);
int command_match(const char *, const char *, size_t);
int is_history_digit(char);
size_t command_size(Command);
size_t energy_length(const char *);
size_t history_size(const char *);
size_t read_line();
size_t second_arg_size(Command c, const char *str);
void init_if_one(Energy **, Energy **);
int two_arg_energy(const char *, const char *);
int two_arg_equal(const char *, const char *);
int parse_line(const char *);
int exec_one_arg(const char *str, Command c, size_t s);
int buffer_enlarge();
void buffer_readjust();
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
		default :
			assert(0);
	}
	return 0;
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
	buffer_readjust();
	size_t ans;
	for (ans = 1; ; ++ans) {
		if (ans >= buffer_size)
			buffer_enlarge();
		buffer[ans - 1] = ' ';
		buffer[ans] = '\0';
		int c = getchar();
		if (c == EOF) return 0;
		if (c == '\n') break;
		buffer[ans - 1] = (char) c;
	}
	return ans;
}

size_t second_arg_size(Command c, const char *str) {
	size_t ans = 0;
	if (c == EQUAL)
		ans = history_size(str);
	if (c == ENERGY)
		ans = energy_length(str);
	return ans;
}

int exec_two_arg(const char *str, Command c, size_t s1, size_t s2) {
	assert(s1 && s2);
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
			return 1;
	}
}

void init_if_one(Energy **e1, Energy **e2) {
	if ((*e1 && *e2) || (!*e1 && !*e2))
		return;
	if (*e1 && !*e2)
		init_if_one(e2, e1);
	else
		*e1 = energy_init(0);
}

int two_arg_energy(const char *arg1, const char *arg2) {
	Node *t = tree_find_split(root, arg1);
	if (!t) return 1;
	if (!strcmp(arg2, MAX_ENERGY)) {
		add_energy(t, energy_convert(UINT64_MAX));
	} else {
		uint64_t a2 = strtoull(arg2, NULL, 10);
		if (a2 != 0 && a2 < UINT64_MAX)
			add_energy(t, energy_convert(a2));
		else
			return 1;
	}
	return 0;
}

int two_arg_equal(const char *arg1, const char *arg2) {
	Node *t1, *t2;
	Energy *e1, *e2;
	t1 = tree_find_split(root, arg1);
	t2 = tree_find_split(root, arg2);
	if (!t1 || !t2)
		return 1;
	e1 = get_energy(t1);
	e2 = get_energy(t2);
	if (!e1 && !e2)
		return 1;
	init_if_one(&e1, &e2);
	energy_union(e1, e2);
	return 0;
}

int parse_line(const char *str) {
	size_t arg1_size, arg2_size = 0;
	Command c = parse_command(str);
	if (c == ERROR) return 1;
	str += command_size(c);
	arg1_size = history_size(str);
	if (!arg1_size)
		return 1;
	arg2_size = second_arg_size(c, str + arg1_size);
	if (str[arg1_size + arg2_size] != '\0') return 1;
	if (arg2_size)
		return exec_two_arg(str, c, arg1_size, arg2_size);
	else
		return exec_one_arg(str, c, arg1_size);
}

int exec_one_arg(const char *str, Command c, size_t s) {
	assert(s);
	char arg[s];
	strncpy(arg, str, s - 1);
	arg[s - 1] = '\0';
	switch (c) {
		case (DECLARE) : {
			tree_insert(root, arg);
			return 0;
		}
		case (ENERGY) : {
			Node *t = tree_find_exact(root, arg);
			if (!t)
				return 1;
			energy_print(get_energy(t));
			return 2;
		}
		case (REMOVE) : {
			//todo
			if (str[1] == ' ') {
			
			}
			tree_remove(root, arg);
			return 0;
		}
		case (VALID) : {
			puts(tree_find_exact(root, arg) ? "YES" : "NO");
			return 2;
		}
		default:
			return 1;
	}
}

int buffer_enlarge() {
	buffer_size *= 4;
	if (buffer == static_buffer) {
		static_buffer[BASE_BUFFER_SIZE - 1] = '\0';
		buffer = malloc(sizeof(char) * buffer_size);
		if (!buffer)
			return 1;
		strcpy(buffer, static_buffer);
	} else {
		buffer = realloc(buffer, buffer_size);
		if (!buffer)
			return 1;
	}
	return 0;
}

void buffer_readjust() {
	if (buffer == static_buffer)
		return;
	free(buffer);
	buffer = static_buffer;
}

void print_error() {
	fprintf(stderr, "ERROR\n");
}

void print_ok() {
	fprintf(stdout, "OK\n");
}

// linked functions

void initialize() {
	root = tree_init();
}

void read_input() {
	for (;;) {
		int parse_result;
		size_t length = read_line();
		if (length == 0)
			return;
		if ((length == 1) || (static_buffer[0] == '#'))
			continue;
		parse_result = parse_line(static_buffer);
		if (parse_result == 0)
			print_ok();
		if (parse_result == 1)
			print_error();
	}
}
