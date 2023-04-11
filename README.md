# ushell
Microshell is a small shell for embedded systems written in C89 without dynamic memory allocations and libc (freestanding).

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

tl;dr -> [arduino port](https://github.com/ilya-sotnikov/ushell/blob/main/port/arduino/ushell_serial.ino)

1. Add ushell.c and ushell.h to your project and #include "ushell.h".
2. Define a function to transmit 1 char (byte) over the interface of your choice (UART, I2C, SPI, etc.). Don't redeclare it.
```C
void ushell_putchar(char chr);
```
3. Provide a command list (name, description, function). Functions must have this prototype:
```C
void function_name(int argc, char *argv[]);
```
5. Initialize ushell like this:
```C
const ushell_command_t commands[] = {
        { "test", "prints \"test\"", &test },
        { "print_args", "prints all args", &print_args },
        { "add2ints", "adds 2 int args", &add2ints },
};
USHELL_INIT(commands);
```
5. Call this function when new data is available in the interface of your choice.
```C
void ushell_process(char chr);
```

You can check examples from the "port" directory.

If you are building it on PC using cmake it just runs tests in main.c. To build and run tests:

1. `git clone https://github.com/ilya-sotnikov/ushell`
2. `cd ushell`
3. `mkdir build && cd build`
4. `cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja ../src && cmake --build .`
5. `./ushell`

If you use it for MCUs there's an [arduino port](https://github.com/ilya-sotnikov/ushell/blob/main/port/arduino/ushell_serial.ino) you can use as an example.

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

Debug build uses sanitizers (address, undefined, leak) by default.
