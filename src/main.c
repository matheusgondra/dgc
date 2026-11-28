#include "colors.h"
#include "command.h"
#include "dgc/string.h"
#include "utils.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif

static void print_help();
static void print_version();

static int finish_main(int exit_code) {
  command_shutdown();
  return exit_code;
}

int main(int argc, char *argv[]) {
#ifdef _WIN32
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hOut != INVALID_HANDLE_VALUE) {
    DWORD dwMode = 0;
    if (GetConsoleMode(hOut, &dwMode)) {
      dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
      SetConsoleMode(hOut, dwMode);
    }
  }
#endif
  if (argc < 2) {
    print_help();
    return finish_main(EXIT_FAILURE);
  }

  bool is_flag = argv[1][0] == '-';
  if (!is_flag) {
    string_t command_name = string_from(argv[1]);
    const Command *cmd = command_find(&command_name);
    string_free(&command_name);

    if (!cmd) {
      fprintf(stderr, "Error: Unknown command \"%s\"\n", argv[1]);
      print_help();
      return finish_main(EXIT_FAILURE);
    }

    int status = cmd->handler(argc - 1, argv + 1);
    return finish_main(status);
  }

  static const struct option long_opts[] = {
      {"help", no_argument, nullptr, 'h'},
      {"version", no_argument, nullptr, 'v'},
      {nullptr, 0, nullptr, 0}};

  optind = 1;
  opterr = 0;
  int opt = -1;
  while ((opt = getopt_long(argc, argv, "+hv", long_opts, nullptr)) != -1) {
    switch (opt) {
    case 'h':
      print_help();
      return finish_main(EXIT_SUCCESS);
    case 'v':
      print_version();
      return finish_main(EXIT_SUCCESS);
    default:
      fprintf(stderr, "Error: Unknown global option.\n\n");
      print_help();
      return finish_main(EXIT_FAILURE);
    }
  }

  print_help();
  return finish_main(EXIT_FAILURE);
}

static void print_help() {
  size_t count = 0;
  const Command *commands = command_all(&count);

  println(BOLD("Usage:") " %s <command> [OPTIONS]", PROJECT_NAME);
  println(BOLD("       ") "%s [GLOBAL_OPTIONS]", PROJECT_NAME);
  println("");
  println(BOLD("Commands:"));
  for (size_t i = 0; i < count; ++i) {
    const Command *command = &commands[i];
    const char *name = string_to_cstr(&command->name);
    const char *usage = string_to_cstr(&command->usage);
    const char *description = string_to_cstr(&command->description);

    println("  " COLORIZE(COLOR_CYAN, "%-14s") " %-28s %s", name, usage,
            description);
  }
  println("");
  println(BOLD("Global Options:"));
  println("  " COLORIZE(
      COLOR_CYAN, "-h, --help") "     Display this help message and exit");
  println("  " COLORIZE(
      COLOR_CYAN,
      "-v, --version") "  Display the version information and exit");
}

static void print_version() {
  println(BOLD("%s") " version " COLORIZE(COLOR_GREEN, "%s"), PROJECT_NAME,
          PROJECT_VERSION);
}