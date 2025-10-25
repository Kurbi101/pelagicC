#ifndef IO_H
#define IO_H

#include "string.h"

typedef str (*formatter)(void *);

void print(const char *s, ...);
void println(const char *s, ...);

str	 format(const char *fmt, ...);
void add_formatter(str name, formatter func, u64 num_bytes);
#endif
