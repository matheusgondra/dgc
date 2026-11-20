#pragma once

#include <stdio.h>

#define println(format, ...) \
  do { \
    printf(format "\n", ##__VA_ARGS__); \
  } while (0)
