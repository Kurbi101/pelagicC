#include "../io.h"
#include "../string.h"
#include <stdarg.h>
#include <unistd.h>

void print(const char *s, ...) {
	va_list args;
	va_start(args, s);
	clean_str formatted_string = format(s, args);
	write(1, formatted_string.data, formatted_string.len);
}

void println(const char *s, ...) {
	va_list args;
	va_start(args, s);
	print(s, args);
	write(1, "\n", 1);
}
