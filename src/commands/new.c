#include "commands/new.h"
#include "String.h"
#include "colors.h"
#include "command.h"
#include "templates.h"
#include "utils.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  string_t project_name;
  bool force;
} NewOptions;

static bool parse_new_options(int argc, char *argv[], NewOptions *opts,
                              bool *help_requested) {
  static const struct option long_opts[] = {
      {"help", no_argument, nullptr, 'h'},
      {"force", no_argument, nullptr, 'f'},
      {nullptr, 0, nullptr, 0}};

  *opts = (NewOptions){.project_name = STRING_EMPTY, .force = false};
  *help_requested = false;

  optind = 1;
  opterr = 0;

  int opt = -1;
  while ((opt = getopt_long(argc, argv, "+hf", long_opts, nullptr)) != -1) {
    switch (opt) {
    case 'h':
      *help_requested = true;
      return true;

    case 'f':
      opts->force = true;
      break;
    default:
      fprintf(stderr, "Error: Unknown option in new command\n");
      return false;
    }
  }

  if (argc - optind != 1) {
    fprintf(stderr, "Error: Missing or unexpected <project_name>.\n");
    return false;
  }

  opts->project_name = string_from(argv[optind]);

  return true;
}

int handle_new_command(int argc, char *argv[]) {
  NewOptions opts = {.project_name = STRING_EMPTY, .force = false};
  bool help_requested = false;

  bool result = parse_new_options(argc, argv, &opts, &help_requested);
  if (!result) {
    string_free(&opts.project_name);
    print_new_command_help();
    return EXIT_FAILURE;
  }

  if (help_requested) {
    string_free(&opts.project_name);
    print_new_command_help();
    return EXIT_SUCCESS;
  }

  const char *project_name_cstr = string_to_cstr(&opts.project_name);
  if (!generate_project(project_name_cstr, opts.force)) {
    fprintf(stderr, "Failed to generated project: %s\n", project_name_cstr);
    string_free(&opts.project_name);
    return EXIT_FAILURE;
  }

  string_free(&opts.project_name);
  return EXIT_SUCCESS;
}

void print_new_command_help() {
  string_t cmd_name = string_from("new");
  const Command *cmd = command_find(&cmd_name);
  string_free(&cmd_name);

  const char *usage = cmd ? string_to_cstr(&cmd->usage) : "";
  const char *description = cmd ? string_to_cstr(&cmd->description) : "";

  println(BOLD("Usage:") " %s new %s", PROJECT_NAME, usage);
  println("");
  println(BOLD("Description:"));
  println("  %s", description);
  println("");
  println(BOLD("Options:"));
  println("  " COLOR_CYAN "-f, --force" COLOR_RESET
          "    Overwrite if directory already exists");
  println("  " COLOR_CYAN "-h, --help" COLOR_RESET
          "     Display this help message and exit");
  println("");
  println(BOLD("Examples:"));
  println("  %s new my_awesome_project", PROJECT_NAME);
}