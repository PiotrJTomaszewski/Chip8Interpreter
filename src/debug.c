#include <stdio.h>
#include <stdarg.h>

void debug_error_print(char *message) {
    // puts(message);
}

void debug_error_printf(char *format, ...) {
    va_list valist;
    va_start(valist, format);
    // vprintf(format, valist);
    va_end(valist);
    // puts("");
}

void debug_info_print(char *message) {
    // puts(message);
}

void debug_info_printf(char *format, ...) {
    va_list valist;
    va_start(valist, format);
    // vprintf(format, valist);
    va_end(valist);
    // puts("");
}

#include "debug.h"
