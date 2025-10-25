#include "../ds/hash_table.h"
#include "../io.h"
#include "../mem.h"
#include "../string.h"
#include <stdarg.h>
#include <unistd.h>

typedef struct formatter_info {
	formatter fmt;
	u64		  num_bytes;
} formatter_info;

DEFINE_HASH_TABLE(formatter_table, str, formatter_info, str_hash, str_eq);

formatter_table *fmt_table = NULL;

void add_formatter(str name, formatter func, u64 num_bytes) {
	if (!fmt_table) {
		fmt_table = formatter_table_new();
	}
	formatter_table_insert(fmt_table, name, (formatter_info){func, num_bytes});
}

str format(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	str_builder result_builder = str_builder_new_none();
	str_builder_append(&result_builder, *fmt);
	while (*(fmt++)) {
		if (*fmt == '{') {
			str_builder type_builder = str_builder_new_none();
			fmt++;
			while (*fmt != '}') {
				str_builder_append(&type_builder, *fmt);
				fmt++;
				if (*fmt == '\0') {
					// TODO: add error handling for missing closing brace
					abort();
				}
			}
			clean_str type = str_build(&type_builder);
			if (formatter_table_contains(fmt_table, type)) {
				formatter_info info = formatter_table_get(fmt_table, type);
				void		  *arg	= malloc(info.num_bytes);
				mem_copy(arg, args, info.num_bytes);
				args += info.num_bytes;
				str value = info.fmt(arg);
				str_builder_append(&result_builder, &value);
			} else {
				// TODO: add error handling in case of non-existant formatter
				abort();
			}
		} else if (*fmt == '\\') {
			if (*(fmt + 1) == '{') {
				fmt++;
				str_builder_append(&result_builder, '{');
				fmt++;
			}
		} else {
			str_builder_append(&result_builder, *fmt);
		}
	}
	return str_build(&result_builder);
}
