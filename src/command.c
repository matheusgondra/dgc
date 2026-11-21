#include "command.h"
#include "commands/new.h"
#include <string.h>

static const Command commands[] = {{.name = "new",
                                    .description = "Create a new C23 project",
                                    .usage = "[OPTIONS] <project_name>",
                                    .handler = handle_new_command,
                                    .help_handler = print_new_command_help}};

const Command *command_find(const char *name) {
  if (!name) {
    return nullptr;
  }

  const size_t count = sizeof(commands) / sizeof(commands[0]);
  for (size_t i = 0; i < count; ++i) {
    if (strcmp(name, commands[i].name) == 0) {
      return &commands[i];
    }
  }

  return nullptr;
}

const Command *command_all(size_t *out_count) {
  if (out_count) {
    *out_count = sizeof(commands) / sizeof(commands[0]);
  }

  return commands;
}