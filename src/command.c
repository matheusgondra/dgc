#include "command.h"
#include "commands/new.h"
#include "dgc/string.h"
#include <string.h>

constexpr size_t COMMAND_COUNT = 1;

static Command commands[COMMAND_COUNT];
static bool commands_initialized = false;

static void ensure_commands_initialized() {
  if (commands_initialized) {
    return;
  }

  commands[0] =
      (Command){.name = string_from("new"),
                .description = string_from("Create a new C23 project"),
                .usage = string_from("[OPTIONS] <project_name>"),
                .handler = handle_new_command,
                .help_handler = print_new_command_help};

  commands_initialized = true;
}

void command_shutdown() {
  if (!commands_initialized) {
    return;
  }

  for (size_t i = 0; i < COMMAND_COUNT; ++i) {
    Command *command = &commands[i];
    string_free(&command->name);
    string_free(&command->description);
    string_free(&command->usage);
  }

  commands_initialized = false;
}

const Command *command_find(const string_t *name) {
  if (string_is_empty(name)) {
    return nullptr;
  }

  ensure_commands_initialized();

  for (size_t i = 0; i < COMMAND_COUNT; ++i) {
    if (string_equals(name, &commands[i].name)) {
      return &commands[i];
    }
  }

  return nullptr;
}

const Command *command_all(size_t *out_count) {
  ensure_commands_initialized();

  if (out_count) {
    *out_count = COMMAND_COUNT;
  }

  return commands;
}