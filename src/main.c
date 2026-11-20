#include "colors.h"
#include "templates.h"
#include "utils.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

static void print_help(const char *program_name);
static void print_version(void);

int main(int argc, char *argv[]) {
  static const struct option long_opts[] = {
      {"help", no_argument, nullptr, 'h'},
      {"version", no_argument, nullptr, 'v'},
      {nullptr, 0, nullptr, 0}};

  int opt = -1;

  while ((opt = getopt_long(argc, argv, "hv", long_opts, nullptr)) != -1) {
    switch (opt) {
    case 'h':
      print_help(argv[0]);
      return EXIT_SUCCESS;
    case 'v':
      print_version();
      return EXIT_SUCCESS;
    default:
      print_help(argv[0]);
      return EXIT_FAILURE;
    }
  }

  if (optind >= argc) {
    fprintf(stderr, "Error: Missing project name.\n");
    print_help(argv[0]);
    return EXIT_FAILURE;
  }

  const char *project_name = argv[optind];
  if (!generate_project(project_name)) {
    fprintf(stderr, "Failed to generate project: %s\n", project_name);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

static void print_help(const char *program_name) {
  println(COLOR_BOLD "Usage:" COLOR_RESET " %s [OPTIONS] <project_name>\n",
          program_name);
  println(COLOR_BOLD "Description:" COLOR_RESET);
  println("  %s\n", PROJECT_DESCRIPTION);
  println(COLOR_BOLD "Options:" COLOR_RESET);
  println("  " COLOR_CYAN "-h, --help" COLOR_RESET
          "     Display this help message and exit");
  println("  " COLOR_CYAN "-v, --version" COLOR_RESET
          "  Output version information and exit\n");
  println(COLOR_BOLD "Examples:" COLOR_RESET);
  println("  %s my_awesome_project\n", program_name);
}

static void print_version(void) {
  println(COLOR_BOLD "%s" COLOR_RESET " version " COLOR_GREEN "%s" COLOR_RESET,
          PROJECT_NAME, PROJECT_VERSION);
}