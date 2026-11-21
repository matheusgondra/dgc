#pragma once

#include <stddef.h>

typedef int (*CommandHandler)(int argc, char *argv[]);
typedef void (*CommandHelpHandler)();

typedef struct {
  const char *name;
  const char *description;
  const char *usage;
  CommandHandler handler;
  CommandHelpHandler help_handler;
} Command;

const Command *command_find(const char *name);
const Command *command_all(size_t *out_count);