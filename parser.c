#include <limits.h>
#include "parser.h"

// maximum energy, the literal representing UINT64_MAX in decimal
#define MAX_ENERGY "18446744073709551615"

// parser result codes
#define PRINT_OK 0
#define PRINT_ERROR 1
#define PRINT_NOTHING 2

static size_t buffer_size = 16;
static Node *root = NULL;
static char *buffer;

// debug variables declarations
static size_t line_counter = 0;


static const char *accept_energy(const char *str);
static const char *accept_history(const char *str);

static energy_t scan_energy(const char *str);
static int do_declare(char *str);
static int do_energy(char *str);
static int do_equal(char *str);
static int do_remove(char *str);
static int do_valid(char *str);
static int is_history_digit(char);
static int is_declare(const char *str);
static int is_energy(const char *str);
static int is_equal(const char *str);
static int is_remove(const char *str);
static int is_valid(const char *str);
static int read_end(const char *str);
static int parse_line(char *);
static int buffer_enlarge(void);
static int buffer_init(void);
static size_t read_line(void);
static void print_error(void);
static void print_ok(void);
static void init_where_null(Energy **e1, Energy **e2);




void initialize() {
	assert(root == NULL);
	root = tree_init();
}

int read_input() {
	{
		int init_error = buffer_init();
		if (init_error)
			return init_error;
	}
	for (;;) {
		int parse_result;
		size_t length = read_line();
		if (length == 0)
			return 0;
		if ((length == 1) || (buffer[0] == '#'))
			continue;
		if (length == SIZE_MAX) {
			tree_destroy(&root);
			free(buffer);
			return 1;
		}
		if (length - 1 == strnlen(buffer, buffer_size))
			parse_result = parse_line(buffer);
		else
			parse_result = PRINT_ERROR;
		++line_counter;
		switch (parse_result) {
			case (PRINT_NOTHING) : {
				break;
			}
			case (PRINT_OK) : {
				print_ok();
				break;
			}
			case (PRINT_ERROR) : {
				print_error();
				break;
			}
			default: {
				assert(0);
				return -1;
			}
		}
	}
}

void finish(void) {
	int error = tree_destroy(&root);
	free(buffer);
	free(root);
	assert(error == 0);
}

// auxiliary function definitions

static int is_history_digit(char c) {
	return c >= '0' && c < '4';
}


static size_t read_line() {
	size_t ans;
	for (ans = 1; ; ++ans) {
		if (ans >= buffer_size) {
			int out_of_memory = buffer_enlarge();
			if (out_of_memory)
//				todo
				return SIZE_MAX;
		}
		buffer[ans - 1] = '\0';
		int c = fgetc(stdin);
		if (c == EOF) {
			if (ans == 1) {
				return 0;
			} else {
				print_error();
				return 0;
			}
		}
		if (c == '\n') break;
		buffer[ans - 1] = (char) c;
	}
	return ans;
}

static void init_where_null(Energy **e1, Energy **e2) {
	assert(*e1 || *e2);
	if (*e1 && *e2)
		return;
	if (*e1) {
		*e1 = energy_find(*e1);
		*e2 = energy_init(energy_value(*e1));
	} else {
		*e2 = energy_find(*e2);
		*e1 = energy_init(energy_value(*e2));
	}
}

static int parse_line(char *str) {
	if (is_declare(str)) {
		return do_declare(str);
	} else if (is_energy(str)) {
		return do_energy(str);
	} else if (is_equal(str)) {
		return do_equal(str);
	} else if (is_remove(str)) {
		return do_remove(str);
	} else if (is_valid(str)) {
		return do_valid(str);
	} else {
		return PRINT_ERROR;
	}
}

static int buffer_enlarge() {
	buffer_size *= 4;
	char *temp;
	temp = realloc(buffer, buffer_size);
	if (temp) {
		buffer = temp;
		return 0;
	}
	free(buffer);
	return 1;
}

static void print_error() {
	fprintf(stderr, "ERROR\n");
}

static void print_ok() {
	fprintf(stdout, "OK\n");
}

static int is_declare(const char *str) {
	const char *const command = "DECLARE ";
	if (strncmp(str, command, strlen(command)) != 0)
		return 0;
	str += strlen(command);
	str = accept_history(str);
	return read_end(str);
}

static int is_energy(const char *str) {
	const char *const command = "ENERGY ";
	if (strncmp(str, command, strlen(command)) != 0)
		return 0;
	str += strlen(command);
	str = accept_history(str);
	if (read_end(str))
		return 1;
	if (str == NULL || str[0] != ' ')
		return 0;
	++str;
	str = accept_energy(str);
	return read_end(str);
}

static int is_equal(const char *str) {
	const char *const command = "EQUAL ";
	if (strncmp(str, command, strlen(command)) != 0)
		return 0;
	str += strlen(command);
	str = accept_history(str);
	if (str == NULL || str[0] != ' ')
		return 0;
	++str;
	str = accept_history(str);
	return read_end(str);
}

static const char *accept_history(const char *str) {
	if (str == NULL)
		return NULL;
	while (is_history_digit(str[0]))
		++str;
	if (str[0] != '\0' && str[0] != ' ')
		return NULL;
	else
		return str;
}

static int read_end(const char *str) {
	return str && str[0] == '\0';
}

static int is_remove(const char *str) {
	const char *const command = "REMOVE ";
	if (strncmp(str, command, strlen(command)) != 0)
		return 0;
	str += strlen(command);
	str = accept_history(str);
	return read_end(str);
}

static const char *accept_energy(const char *str) {
	if (strncmp(str, MAX_ENERGY, strlen(MAX_ENERGY)) == 0)
		return str + strlen(MAX_ENERGY);
	char *ans;
	// strtoull ignores leading spaces, but we want to fail on those
	if (!isdigit(str[0]))
		return NULL;
	uint64_t value = strtoull(str, &ans, 10);
	if (value < ULLONG_MAX)
		return ans;
	else
		return NULL;
}

static int is_valid(const char *str) {
	const char *const command = "VALID ";
	if (strncmp(str, command, strlen(command)) != 0)
		return 0;
	str += strlen(command);
	str = accept_history(str);
	return read_end(str);
}

static int buffer_init(void) {
	buffer = calloc(buffer_size, sizeof(char));
	if (buffer)
		return 0;
	else
		return 1;
}

static int do_declare(char *str) {
	strtok(str, " ");
	char *arg = strtok(NULL, " ");
	if (arg) {
		int error = tree_insert(root, arg);
		if (error == 0)
			return PRINT_OK;
	}
	return PRINT_ERROR;
}

static energy_t scan_energy(const char *str) {
	return strtoull(str, NULL, 10);
}

static int do_energy(char *str) {
	strtok(str, " ");
	char *arg1 = strtok(NULL, " ");
	Node *t = tree_find(root, arg1);
	char *arg2 = strtok(NULL, " ");
	if (arg2) {
		int error;
		energy_t en = scan_energy(arg2);
		if (!t)
			return PRINT_ERROR;
		error = add_energy(t, en);
		switch (error) {
			case (0) :
				return PRINT_OK;
			case (1) :
				return PRINT_ERROR;
			default: {
				assert(0);
				return -1;
			}
		}
	} else {
		if (!t || energy_print(*get_energy(t)))
			return PRINT_ERROR;
		else
			return PRINT_NOTHING;
	}
}

static int do_equal(char *str) {
	strtok(str, " ");
	char *arg1 = strtok(NULL, " ");
	char *arg2 = strtok(NULL, " ");
	Node *t1 = tree_find(root, arg1);
	Node *t2 = tree_find(root, arg2);
	if (strcmp(arg1, arg2) == 0) { // EQUAL with identical arguments
		assert(t1 == t2);
		if (t1) // return no error...
			return PRINT_OK;
		else // ...unless the history doesn't exist
			return PRINT_ERROR;
	}
	if (t1 == NULL || t2 == NULL)
		return PRINT_ERROR;
	Energy **e1 = get_energy(t1);
	Energy **e2 = get_energy(t2);
	if (e1[0] == NULL && e2[0] == NULL)
		return PRINT_ERROR;
	init_where_null(e1, e2);
	energy_union(*e1, *e2);
	return PRINT_OK;
}

int do_remove(char *str) {
	strtok(str, " ");
	char *arg1 = strtok(NULL, " ");
	if (tree_find(root, arg1)) {
		tree_remove(root, arg1);
		return PRINT_OK;
	} else {
		// removing a nonexistent history is valid
		return PRINT_OK;
	}
}

int do_valid(char *str) {
	strtok(str, " ");
	char *arg1 = strtok(NULL, " ");
	if (arg1) {
		puts(tree_find(root, arg1) ? "YES" : "NO");
		return PRINT_NOTHING;
	} else {
		return PRINT_ERROR;
	}
}

