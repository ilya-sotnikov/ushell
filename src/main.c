#include "ushell.h"

#include "unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ushell_putchar(char chr);
void print_string(int argc, char *argv[]);
void add2ints(int argc, char *argv[]);

static int global_int;

void setUp(void)
{
}

void tearDown(void)
{
}

void ushell_putchar(char chr)
{
	putchar(chr);
}

void print_string(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	printf("test\r\n");
}

void add2ints(int argc, char *argv[])
{
	int a, b, sum;

	if (argc != 3)
		return;

	a = atoi(argv[1]);
	b = atoi(argv[2]);
	sum = a + b;

	global_int = sum;

	printf("%d + %d = %d\r\n", a, b, sum);
}

#define PROCESS_STR(str)                                        \
	do {                                                    \
		const char *input = str;                        \
		const unsigned long input_length = strlen(str); \
		for (i = 0; i < input_length; ++i)              \
			ushell_process(input[i]);               \
	} while (0)

void test_inputs(void)
{
	unsigned long i;
	char long_str[500];
	const unsigned long long_str_len =
		sizeof(long_str) / sizeof(long_str[0]);

	PROCESS_STR("");

	PROCESS_STR("add 1 2\n");
	TEST_ASSERT_EQUAL(3, global_int);
	global_int = 0;

	PROCESS_STR("add  5      0\n");
	TEST_ASSERT_EQUAL(5, global_int);
	global_int = 0;

	PROCESS_STR("add   5      1  \n");
	TEST_ASSERT_EQUAL(6, global_int);
	global_int = 0;

	for (i = 0; i < long_str_len; ++i)
		long_str[i] = 'x';
	long_str[long_str_len - 2] = '\n';
	long_str[long_str_len - 1] = '\0';
	PROCESS_STR(long_str);
	/* TODO assert err */

	PROCESS_STR("add\010d 1 2\n");
	TEST_ASSERT_EQUAL(3, global_int);
	global_int = 0;

	PROCESS_STR("add\010\010\010\010\010add 1 2\n");
	TEST_ASSERT_EQUAL(3, global_int);
	global_int = 0;

	for (i = 0; i < long_str_len; ++i)
		long_str[i] = '\010';
	long_str[long_str_len - 2] = '\n';
	long_str[long_str_len - 1] = '\0';
	PROCESS_STR(long_str);
}

int main(void)
{
	const ushell_command_t commands[] = {
		{ "print", "prints \"test\"", &print_string },
		{ "add", "adds 2 ints", &add2ints },
	};

	ushell_init(commands, sizeof(commands) / sizeof(commands[0]));

	UNITY_BEGIN();

	RUN_TEST(test_inputs);

	return UNITY_END();
}
