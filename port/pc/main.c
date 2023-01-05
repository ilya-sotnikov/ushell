#include "ushell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_args(int argc, char *argv[])
{
	int i;
	for (i = 0; i < argc; ++i)
		printf("%s\r\n", argv[i]);
}

void add2ints(int argc, char *argv[])
{
	int a, b, sum;

	if (argc != 3)
		return;

	a = atoi(argv[1]);
	b = atoi(argv[2]);
	sum = a + b;

	printf("%d + %d = %d\r\n", a, b, sum);
}

void test(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	printf("test\r\n");
}

void ushell_putchar(char chr)
{
	putchar(chr);
}

int main(void)
{
	char c;

	const ushell_command_t commands[] = {

		{ "test", "prints \"test\"", &test },
		{ "print_args", "prints all args", &print_args },
		{ "add2ints", "adds 2 int args", &add2ints },

	};
	const size_t commands_count = sizeof(commands) / sizeof(commands[0]);

	ushell_init(commands, commands_count, &ushell_putchar);

	while ((c = getchar()) != EOF)
		ushell_process(c);

	return 0;
}
