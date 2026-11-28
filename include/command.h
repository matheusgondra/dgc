#pragma once

#include "dgc/string.h"
#include <stddef.h>

typedef int (*CommandHandler)(int argc, char *argv[]);
typedef void (*CommandHelpHandler)();

typedef struct {
  string_t name;
  string_t description;
  string_t usage;
  CommandHandler handler;
  CommandHelpHandler help_handler;
} Command;

const Command *command_find(const string_t *name);
void command_shutdown();
const Command *command_all(size_t *out_count);