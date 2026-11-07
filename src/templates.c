#include "templates.h"
#include "colors.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32

#include <direct.h>

#define create_dir(path) _mkdir(path)

#else

#include <sys/stat.h>

#define create_dir(path) mkdir(path, 0755)

#endif

static const char cmake_lists_template[] = {
#embed "templates/cmake-lists.template"
    , 0};

static const char cmake_presets_template[] = {
#embed "templates/cmake-presets.template"
    , 0};

static const char vcpkg_template[] = {
#embed "templates/vcpkg.template"
    , 0};

static const char library_h_template[] = {
#embed "templates/library-h.template"
    , 0};

static const char library_c_template[] = {
#embed "templates/library-c.template"
    , 0};

static const char main_c_template[] = {
#embed "templates/main-c.template"
    , 0};

static const char gitignore_template[] = {
#embed "templates/gitignore.template"
    , 0};

static const char readme_template[] = {
#embed "templates/readme.template"
    , 0};

static void write_processed_template(const char *path, const char *content,
                                     const char *project_name) {
  FILE *file = fopen(path, "w");
  if (!file) {
    fprintf(stderr, "Failed to create file: %s\n", path);
    return;
  }

  const char *cursor = content;
  const char *token = "$NAME$";
  size_t token_len = strlen(token);

  while (*cursor != '\0') {
    if (strncmp(cursor, token, token_len) == 0) {
      fputs(project_name, file);
      cursor += token_len;
    } else {
      fputc(*cursor, file);
      cursor++;
    }
  }

  fclose(file);
}

[[nodiscard]] bool generate_project(const char *project_name) {
  char path_buffer[512];

  printf("\n" COLOR_BOLD "🚀 Generating C23 project: %s" COLOR_RESET "\n\n",
         project_name);

  if (create_dir(project_name) != 0 && errno != EEXIST) {
    perror("Failed to create project directory");
    return false;
  }

  create_dir(
      snprintf(path_buffer, sizeof(path_buffer), "%s/include", project_name) > 0
          ? path_buffer
          : "");
  create_dir(
      snprintf(path_buffer, sizeof(path_buffer), "%s/src", project_name) > 0
          ? path_buffer
          : "");
  create_dir(
      snprintf(path_buffer, sizeof(path_buffer), "%s/.vscode", project_name) > 0
          ? path_buffer
          : "");

  snprintf(path_buffer, sizeof(path_buffer), "%s/vcpkg.json", project_name);
  write_processed_template(path_buffer, vcpkg_template, project_name);

  snprintf(path_buffer, sizeof(path_buffer), "%s/CMakeLists.txt", project_name);
  write_processed_template(path_buffer, cmake_lists_template, project_name);

  snprintf(path_buffer, sizeof(path_buffer), "%s/CMakePresets.json",
           project_name);
  write_processed_template(path_buffer, cmake_presets_template, project_name);

  snprintf(path_buffer, sizeof(path_buffer), "%s/.gitignore", project_name);
  write_processed_template(path_buffer, gitignore_template, project_name);

  snprintf(path_buffer, sizeof(path_buffer), "%s/README.md", project_name);
  write_processed_template(path_buffer, readme_template, project_name);

  snprintf(path_buffer, sizeof(path_buffer), "%s/include/library.h",
           project_name);
  write_processed_template(path_buffer, library_h_template, project_name);

  snprintf(path_buffer, sizeof(path_buffer), "%s/src/library.c", project_name);
  write_processed_template(path_buffer, library_c_template, project_name);

  snprintf(path_buffer, sizeof(path_buffer), "%s/src/main.c", project_name);
  write_processed_template(path_buffer, main_c_template, project_name);

  printf("  " COLOR_CYAN "📂 %s/" COLOR_RESET "\n", project_name);
  printf("  ├── " COLOR_CYAN "include/" COLOR_RESET "library.h\n");
  printf("  ├── " COLOR_CYAN "src/" COLOR_RESET "library.c\n");
  printf("  ├── " COLOR_CYAN "src/" COLOR_RESET "main.c\n");
  printf("  ├── CMakeLists.txt\n");
  printf("  ├── CMakePresets.json\n");
  printf("  ├── vcpkg.json\n");
  printf("  ├── .gitignore\n");
  printf("  └── README.md\n");

  printf("\n" COLOR_GREEN "✨ Project successfully created!" COLOR_RESET "\n");
  printf("👉 Next steps:\n");
  printf("   cd %s\n", project_name);
  printf("   cmake --preset default\n");
  printf("   cmake --build --preset default\n\n");

  return true;
}