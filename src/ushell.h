#ifndef USHELL_H
#define USHELL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#define USHELL_ARGC_MAX 10
#define USHELL_RX_BUFFER_SIZE 255
#define USHELL_PROMPT "$ "

typedef struct {
	const char *name;
	const char *description;
	void (*const func)(int argc, char *argv[]);
} ushell_command_t;

void ushell_putchar(char chr); /* define this function */
void ushell_init(const ushell_command_t *commands, size_t commands_cnt);
void ushell_process(char chr);

#define USHELL_INIT(commands)                                   \
	do {                                                    \
		const size_t ushell_commands_count =            \
			sizeof(commands) / sizeof(commands[0]); \
		ushell_init(commands, ushell_commands_count);   \
	} while (0)

#ifdef __cplusplus
}
#endif

#endif /* USHELL_H */
