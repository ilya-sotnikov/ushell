#include "ushell.h"

#include <ctype.h>
#include <string.h>

#define ASCII_BACKSPACE 0x08
#define ASCII_DEL 0x7f

static struct {
	char rx[USHELL_RX_BUFFER_SIZE];
	int rx_ptr;

	int argc;
	char *argv[USHELL_ARGC_MAX];

	const ushell_command_t *commands;
	size_t commands_cnt;

	void (*putchar)(char chr);
} ushell;

static void ushell_print(const char *str);
static void ushell_println(const char *str);
static void ushell_print_help(void);
static void ushell_receive(char c);
static void ushell_parse(void);
static void ushell_exec(void);

void ushell_init(const ushell_command_t *commands, size_t commands_cnt,
		 void (*ushell_putchar)(char chr))
{
	ushell.commands = commands;
	ushell.commands_cnt = commands_cnt;
	ushell.putchar = ushell_putchar;
	ushell_print_help();
	ushell_print(USHELL_PROMPT);
}

void ushell_print(const char *str)
{
	unsigned long len = strlen(str);
	unsigned long i;
	for (i = 0; i < len; ++i)
		ushell.putchar(str[i]);
}

void ushell_println(const char *str)
{
	ushell_print(str);
	ushell_print("\r\n");
}

void ushell_print_help(void)
{
	size_t i;
	ushell_println("ushell: commands available:");
	for (i = 0; i < ushell.commands_cnt; ++i) {
		ushell_print(ushell.commands[i].name);
		ushell_print(" - ");
		ushell_println(ushell.commands[i].description);
	}
	ushell_println("help - list all commands");
}

void ushell_receive(char chr)
{
	if (ushell.rx_ptr >= USHELL_RX_BUFFER_SIZE) {
		ushell.rx_ptr = 0;
		ushell_println("ushell: command is too long");
		return;
	}

	switch (chr) {
	case '\r':
	case '\n':
		if (ushell.rx_ptr > 0) {
			ushell_print("\r\n");
			if (ushell.rx[ushell.rx_ptr - 1] == ' ')
				ushell.rx[ushell.rx_ptr - 1] = '\0';
			ushell.rx[ushell.rx_ptr++] = '\0';
		}
		break;

	case ASCII_BACKSPACE:
	case ASCII_DEL:
		if (ushell.rx_ptr > 0) {
			--ushell.rx_ptr;
			ushell_print("\b \b");
		}
		break;

	case ' ':
		if (ushell.rx_ptr > 0 && ushell.rx[ushell.rx_ptr - 1] != ' ') {
			ushell.rx[ushell.rx_ptr++] = chr;
			ushell.putchar(chr);
		}
		break;

	default:
		ushell.rx[ushell.rx_ptr++] = chr;
		ushell.putchar(chr);
		break;
	}
}

void ushell_parse(void)
{
	int i;
	unsigned long arg_pos;

	if (ushell.rx_ptr == 0)
		return;

	if ((ushell.rx_ptr > 0) && (ushell.rx[ushell.rx_ptr - 1] != '\0'))
		return;

	arg_pos = 0;
	for (i = 0; i < ushell.rx_ptr; ++i) {
		if (ushell.rx[i] == ' ') {
			if (ushell.argc >= USHELL_ARGC_MAX) {
				ushell_println("ushell: argc > ARGC_MAX");
				return;
			}

			ushell.rx[i] = '\0';
			ushell.argv[ushell.argc++] = ushell.rx + arg_pos;
			arg_pos = ++i;
		}
	}
	ushell.argv[ushell.argc++] = ushell.rx + arg_pos;
}

void ushell_exec(void)
{
	size_t i;
	int command_found = 0;

	if (ushell.argc == 0)
		return;

	for (i = 0; i < ushell.commands_cnt; ++i) {
		if (!strcmp(ushell.commands[i].name, ushell.argv[0])) {
			command_found = 1;
			ushell.commands[i].func(ushell.argc, ushell.argv);
		}
	}
	if (!strcmp("help", ushell.argv[0])) {
		command_found = 1;
		ushell_print_help();
	}

	if (!command_found)
		ushell_println("ushell: command not found, use \"help\"");

	ushell_print(USHELL_PROMPT);

	ushell.rx_ptr = 0;
	ushell.argc = 0;
}

void ushell_process(char chr)
{
	ushell_receive(chr);
	ushell_parse();
	ushell_exec();
}
