#include "dgc/string.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const string_t STRING_EMPTY = {.chars = "", .length = 0};

static bool is_null_string(const string_t *string) {
  return !string || !string->chars;
}

static bool is_empty_string(const string_t *string) {
  return is_null_string(string) || string->length == 0 ||
         string->chars == STRING_EMPTY.chars;
}

string_t string_from(const char *chars) {
  if (!chars) {
    return STRING_EMPTY;
  }

  size_t length = strlen(chars);
  if (length == 0) {
    return STRING_EMPTY;
  }

  if (length > UINT32_MAX) {
    return STRING_EMPTY;
  }

  return string_from_n(chars, (uint32_t)length);
}

string_t string_from_n(const char *chars, uint32_t length) {
  if (!chars || length == 0) {
    return STRING_EMPTY;
  }

  char *buffer = malloc(length + 1);
  if (!buffer) {
    return STRING_EMPTY;
  }

  memcpy(buffer, chars, length);
  buffer[length] = '\0';

  return (string_t){.chars = buffer, .length = length};
}

string_t string_clone(const string_t *string) {
  if (is_null_string(string) || is_empty_string(string)) {
    return STRING_EMPTY;
  }

  return string_from_n(string->chars, string->length);
}

size_t string_length(const string_t *string) {
  if (is_null_string(string)) {
    return 0;
  }

  return string->length;
}

bool string_is_empty(const string_t *string) {
  return is_null_string(string) || is_empty_string(string);
}

const char *string_to_cstr(const string_t *string) {
  if (is_null_string(string)) {
    return "";
  }

  return string->chars;
}

bool string_equals(const string_t *string1, const string_t *string2) {
  if (string1 == string2) {
    return true;
  }

  if (!string1 || !string2) {
    return false;
  }

  if (string1->length != string2->length) {
    return false;
  }

  if (string1->length == 0) {
    return true;
  }

  if (!string1->chars || !string2->chars) {
    return false;
  }

  return memcmp(string1->chars, string2->chars, (size_t)string1->length) == 0;
}

bool string_equals_cstr(const string_t *string, const char *cstr) {
  if (!string || !cstr) {
    return false;
  }

  if (string->length == 0) {
    return cstr[0] == '\0';
  }

  if (!string->chars) {
    return false;
  }

  size_t cstr_length = strlen(cstr);
  if ((size_t)string->length != cstr_length) {
    return false;
  }

  return memcmp(string->chars, cstr, cstr_length) == 0;
}

void string_print(const string_t *string) {
  if (is_null_string(string) || is_empty_string(string)) {
    return;
  }

  printf("%.*s", string->length, string->chars);
}

void string_println(const string_t *string) {
  if (is_null_string(string) || is_empty_string(string)) {
    return;
  }

  printf("%.*s\n", string->length, string->chars);
}

void string_free(string_t *string) {
  if (is_null_string(string) || string->chars == STRING_EMPTY.chars) {
    return;
  }

  free(string->chars);
  string->chars = nullptr;
  string->length = 0;
}
