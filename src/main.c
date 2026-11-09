#include "colors.h"
#include "templates.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

static void print_help(const char *program_name) {
  printf(COLOR_BOLD "Usage:" COLOR_RESET " %s [OPTIONS] <project_name>\n\n",
         program_name);
  printf(COLOR_BOLD "Description:" COLOR_RESET "\n");
  printf("  %s\n\n", PROJECT_DESCRIPTION);
  printf(COLOR_BOLD "Options:" COLOR_RESET "\n");
  printf("  " COLOR_CYAN "-h, --help" COLOR_RESET
         "     Display this help message and exit\n");
  printf("  " COLOR_CYAN "-v, --version" COLOR_RESET
         "  Output version information and exit\n\n");
  printf(COLOR_BOLD "Examples:" COLOR_RESET "\n");
  printf("  %s my_awesome_project\n\n", program_name);
}

static void print_version(void) {
  printf(COLOR_BOLD "%s" COLOR_RESET " version " COLOR_GREEN "%s" COLOR_RESET
                    "\n",
         PROJECT_NAME, PROJECT_VERSION);
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
    print_help(argv[0]);
    return EXIT_FAILURE;
  }

  if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
    print_help(argv[0]);
    return EXIT_SUCCESS;
  }

  if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
    print_version();
    return EXIT_SUCCESS;
  }

  const char *project_name = argv[1];
  if (!generate_project(project_name)) {
    fprintf(stderr, "Failed to generate project: %s\n", project_name);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}