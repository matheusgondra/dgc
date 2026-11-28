#include "templates.h"
#include "colors.h"
#include "dgc/string.h"
#include "utils.h"
#include <errno.h>
#include <stdint.h>
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

static const char String_h_template[] = {
#embed "../include/dgc/string.h"
    , 0};

static const char String_c_template[] = {
#embed "./dgc/string.c"
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

[[nodiscard]]
static string_t path_join(const string_t *dir, const string_t *file) {
  const char *dir_str = string_to_cstr(dir);
  const char *file_str = string_to_cstr(file);

  int needed = snprintf(nullptr, 0, "%s/%s", dir_str, file_str);
  if (needed <= 0 || (uintmax_t)needed > UINT32_MAX) {
    fprintf(stderr, "Error: Invalid or too long path for \"%s/%s\"\n", dir_str,
            file_str);
    return STRING_EMPTY;
  }

  size_t total_len = (size_t)needed;
  char *buffer = (char *)malloc(total_len + 1);
  if (!buffer) {
    return STRING_EMPTY;
  }

  int written = snprintf(buffer, total_len + 1, "%s/%s", dir_str, file_str);
  if (written < 0 || (size_t)written != total_len) {
    free(buffer);
    return STRING_EMPTY;
  }

  string_t result = string_from_n(buffer, (size_t)total_len);
  free(buffer);

  return result;
}

static bool ensure_dir(const string_t *path, bool allow_existing) {
  const char *path_str = string_to_cstr(path);
  if (create_dir(path_str) == 0) {
    return true;
  }

  if (allow_existing && errno == EEXIST) {
    return true;
  }

  perror("Failed to create directory");

  return false;
}

[[nodiscard]]
static bool write_processed_template(const string_t *out_path,
                                     const char *content,
                                     const string_t *project_name) {
  const char *out_path_cstr = string_to_cstr(out_path);

  FILE *file = fopen(out_path_cstr, "w");
  if (!file) {
    fprintf(stderr, "Failed to create file: %s\n", out_path_cstr);
    return false;
  }

  const char *cursor = content ? content : "";
  const char *token = "$NAME$";
  size_t token_len = strlen(token);
  const char *project_name_cstr = string_to_cstr(project_name);

  int result = 0;

  while (*cursor != '\0') {
    if (strncmp(cursor, token, token_len) == 0) {
      result = fputs(project_name_cstr, file);
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
bool generate_project(const string_t *project_name, bool force) {
  if (string_is_empty(project_name)) {
    fprintf(stderr, "Error: Project name cannot be empty.\n");
    return false;
  }

  const char *project_name_cstr = string_to_cstr(project_name);

  println("");
  println(BOLD("🚀 Generating C23 project: %s"), project_name_cstr);
  println("");

  if (!ensure_dir(project_name, force)) {
    return false;
  }

  string_t subdirs[] = {string_from("include"), string_from("include/dgc"),
                        string_from("src"), string_from("src/dgc")};
  const size_t subdir_count = sizeof(subdirs) / sizeof(subdirs[0]);

  for (size_t i = 0; i < subdir_count; ++i) {
    string_t full_subdir = path_join(project_name, &subdirs[i]);
    bool subdir_result = ensure_dir(&full_subdir, force);
    string_free(&full_subdir);

    if (!subdir_result) {
      return false;
    }
  }

  for (size_t i = 0; i < subdir_count; ++i) {
    string_free(&subdirs[i]);
  }

  const TemplateFile files[] = {{"vcpkg.json", vcpkg_template},
                                {"CMakeLists.txt", cmake_lists_template},
                                {"CMakePresets.json", cmake_presets_template},
                                {"include/dgc/string.h", String_h_template},
                                {"src/dgc/string.c", String_c_template},
                                {"src/main.c", main_c_template},
                                {".gitignore", gitignore_template},
                                {"README.md", readme_template}};
  const size_t file_count = sizeof(files) / sizeof(files[0]);

  for (size_t i = 0; i < file_count; ++i) {
    TemplateFile file = files[i];
    string_t relative_path = string_from(file.relative_path);

    string_t full_file = path_join(project_name, &relative_path);
    bool write_result =
        write_processed_template(&full_file, file.content, project_name);

    string_free(&full_file);
    string_free(&relative_path);

    if (!write_result) {
      return false;
    }
  }

  println("  " COLORIZE(COLOR_CYAN, "📂 %s"), project_name_cstr);
  for (size_t i = 0; i < file_count; ++i) {
    const char *branch = i + 1 == file_count ? "└──" : "├──";
    println("  %s %s", branch, files[i].relative_path);
  }

  println("");
  println(COLORIZE(COLOR_GREEN, "✨ Project successfully created!"));
  println("👉 Next steps:");
  println("   cd %s", project_name_cstr);
  println("   cmake --preset default");
  println("   cmake --build --preset default");
  println("");

  return true;
}