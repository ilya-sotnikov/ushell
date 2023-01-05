#include "ushell.h"

void print_args(int argc, char *argv[])
{
  int i;
  for (i = 0; i < argc; ++i)
    Serial.println(argv[i]);
}

void toggle_led(int argc, char *argv[])
{
  (void)argc;
  (void)argv;
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); 
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  
  const ushell_command_t commands[] = {

    { "print_args", "prints all args", &print_args },
    { "toggle_led",  "toggle LED",     &toggle_led },

  };
  const size_t commands_count = sizeof(commands) / sizeof(commands[0]);

  ushell_init(commands, commands_count, [](int chr) { Serial.write(chr); });
}

void loop() {
  if (Serial.available()) {
    ushell_process(Serial.read());
  }
}
