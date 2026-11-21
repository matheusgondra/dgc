#pragma once

#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_BOLD    "\033[1m"

#define BOLD(text) COLOR_BOLD text COLOR_RESET
#define COLORIZE(color, text) color text COLOR_RESET