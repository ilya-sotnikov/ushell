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

void ushell_init(const ushell_command_t *commands, size_t commands_cnt,
		 void (*ushell_putchar)(char chr));
void ushell_process(char chr);

#ifdef __cplusplus
}
#endif

#endif /* USHELL_H */
