#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct {
    char *chars;
    uint32_t length;
} string_t;

extern const string_t STRING_EMPTY;

string_t string_from(const char *chars);
string_t string_from_n(const char *chars, uint32_t length);
string_t string_clone(const string_t *string);

size_t string_length(const string_t *string);
bool string_is_empty(const string_t *string);
const char *string_to_cstr(const string_t *string);

bool string_equals(const string_t *string1, const string_t *string2);
bool string_equals_cstr(const string_t *string, const char *cstr);

void string_print(const string_t *string);
void string_println(const string_t *string);
void string_free(string_t *string);
