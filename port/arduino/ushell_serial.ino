#include "ushell.h"

// define this function, don't redeclare it
void ushell_putchar(char chr)
{
  Serial.write(chr);
}

// example function
void print_args(int argc, char *argv[])
{
  int i;
  for (i = 0; i < argc; ++i)
    Serial.println(argv[i]);
}

// example function
void toggle_led(int argc, char *argv[])
{
  (void)argc;
  (void)argv;
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); 
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  
  // add static to ensure this array will not get destroyed
  // "name", "help string", function address
  static const ushell_command_t commands[] = {
    { "print_args", "prints all args", &print_args },
    { "toggle_led",  "toggle LED",     &toggle_led },
  };

  ushell_init(commands, sizeof(commands) / sizeof(commands[0]));
}

void loop() {
  if (Serial.available())
    ushell_process(Serial.read()); // 1 char
}
