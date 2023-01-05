# ushell
Microshell is a small shell for embedded systems written in C89 without dynamic memory allocations.

## Example
You can provide any commands.
```
ushell: commands available:
test - prints "test"
print_args - prints all args
add2ints - adds 2 int args
help - list all commands

$ add2ints 1 2
1 + 2 = 3
```

## Usage
1. Add ushell.c and ushell.h to your project and #include "ushell.h".
2. Provide a function to transmit 1 char (byte) over the interface of your choice (UART, I2C, SPI, etc.).
```C
void ushell_putchar(char chr)
```
3. Provide a command list (name, description, function). Functions must have this prototype:
```C
void function_name(int argc, char *argv[])
```
5. Initialize ushell like this:
```C
const ushell_command_t commands[] = {
        { "test", "prints \"test\"", &test },
        { "print_args", "prints all args", &print_args },
        { "add2ints", "adds 2 int args", &add2ints },
};
const size_t commands_count = sizeof(commands) / sizeof(commands[0]);
ushell_init(commands, commands_count, &ushell_putchar);
```
5. Call this function when new data is available in the interface of your choice.
```C
void ushell_process(char chr);
```

You can check examples from the "port" directory. There's a cmake file for the PC port.
Note that if you are testing it on the PC you should enable raw input and disable echo in your terminal.

If you use it for MCUs there's an Arduino port you can use as an example.

There's also an implicit "help" function which prints all available functions and their descriptions.

Microshell should work basically everywhere (if there is a C compiler).

## License
Licensed under either of
 * Apache License, Version 2.0
   ([LICENSE-APACHE](LICENSE-APACHE) or http://www.apache.org/licenses/LICENSE-2.0)
 * MIT license
   ([LICENSE-MIT](LICENSE-MIT) or http://opensource.org/licenses/MIT)
at your option.

## Contribution
Unless you explicitly state otherwise, any contribution intentionally submitted
for inclusion in the work by you, as defined in the Apache-2.0 license, shall be
dual licensed as above, without any additional terms or conditions.

Feel free to add features or fix bugs. This project uses the Linux kernel coding style, you 
can use the .clang-format file from [here](https://raw.githubusercontent.com/torvalds/linux/master/.clang-format).

You can also use sanitizers (address or UB). To do so pass -DSANITIZER=ADDR or -DSANITIZER=UB to cmake.
