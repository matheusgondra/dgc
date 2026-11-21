#include "templates.h"
#include "colors.h"
#include "utils.h"
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

typedef struct {
  const char *relative_path;
  const char *content;
} TemplateFile;

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

static bool path_join(char *dst, size_t dst_size, const char *dir,
                      const char *file) {
  int written = snprintf(dst, dst_size, "%s/%s", dir, file);
  if (written < 0 || (size_t)written >= dst_size) {
    fprintf(stderr, "Error: Path too long for \"%s/%s\"\n", dir, file);
    return false;
  }

  return true;
}

static bool ensure_dir(const char *path, bool allow_existing) {
  if (create_dir(path) == 0) {
    return true;
  }

  if (allow_existing && errno == EEXIST) {
    return true;
  }

  perror("Failed to create directory");

  return false;
}

[[nodiscard]]
static bool write_processed_template(const char *path, const char *content,
                                     const char *project_name) {
  FILE *file = fopen(path, "w");
  if (!file) {
    fprintf(stderr, "Failed to create file: %s\n", path);
    return false;
  }

  const char *cursor = content;
  const char *token = "$NAME$";
  size_t token_len = strlen(token);

  int result = 0;

  while (*cursor != '\0') {
    if (strncmp(cursor, token, token_len) == 0) {
      result = fputs(project_name, file);
      if (result == EOF) {
        fclose(file);
        return false;
      }

      cursor += token_len;
    } else {
      result = fputc(*cursor, file);
      if (result == EOF) {
        fclose(file);
        return false;
      }

      cursor++;
    }
  }

  result = fclose(file);
  if (result != 0) {
    return false;
  }

  return true;
}

[[nodiscard]]
bool generate_project(const char *project_name, bool force) {
  char path_buffer[512];
  char file_buffer[512];

  println("");
  println(BOLD("🚀 Generating C23 project: %s"), project_name);
  println("");

  if (!ensure_dir(project_name, force)) {
    return false;
  }

  const char *subdirs[] = {"include", "src"};
  const size_t subdir_count = sizeof(subdirs) / sizeof(subdirs[0]);

  for (size_t i = 0; i < subdir_count; ++i) {
    bool path_result =
        path_join(path_buffer, sizeof(path_buffer), project_name, subdirs[i]);
    bool dir_result = ensure_dir(path_buffer, force);
    if (!path_result || !dir_result) {
      return false;
    }
  }

  const TemplateFile files[] = {{"vcpkg.json", vcpkg_template},
                                {"CMakeLists.txt", cmake_lists_template},
                                {"CMakePresets.json", cmake_presets_template},
                                {"include/library.h", library_h_template},
                                {"src/library.c", library_c_template},
                                {"src/main.c", main_c_template},
                                {".gitignore", gitignore_template},
                                {"README.md", readme_template}};
  const size_t file_count = sizeof(files) / sizeof(files[0]);

  for (size_t i = 0; i < file_count; ++i) {
    bool path_result = path_join(file_buffer, sizeof(file_buffer), project_name,
                                 files[i].relative_path);
    bool write_result =
        write_processed_template(file_buffer, files[i].content, project_name);
    if (!path_result || !write_result) {
      return false;
    }
  }

  println("  " COLORIZE(COLOR_CYAN, "📂 %s"), project_name);
  println("  ├── " COLORIZE(COLOR_CYAN, "include/") "library.h");
  println("  ├── " COLORIZE(COLOR_CYAN, "src/") "library.c");
  println("  ├── " COLORIZE(COLOR_CYAN, "src/") "main.c");
  println("  ├── CMakeLists.txt");
  println("  ├── CMakePresets.json");
  println("  ├── vcpkg.json");
  println("  ├── .gitignore");
  println("  └── README.md");

  println("");
  println(COLORIZE(COLOR_GREEN, "✨ Project successfully created!"));
  println("👉 Next steps:");
  println("   cd %s", project_name);
  println("   cmake --preset default");
  println("   cmake --build --preset default");
  println("");

  return true;
}