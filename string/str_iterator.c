#include "../string.h"

str_iterator str_iterator_new(const str *str) {
	str_iterator it = {str_copy(str), 0};
	return it;
}

char str_iterator_next(str_iterator *it) {
	char c = it->str.data[it->index++];
	return c;
}

bool str_iterator_has_next(str_iterator *it) { return it->index < it->str.len; }
