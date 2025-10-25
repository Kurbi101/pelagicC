#include "../string.h"
#include "../mem.h"
#include <regex.h>
#include <stdio.h>
#include <unistd.h>

void str_cleanup(str *restrict s) {
	free(s->data);
	s->len	= 0;
	s->data = NULL;
}

static inline u64 strlen_helper(const char *restrict data) {
	u64 len = 0;
	while (*data++) len++;
	return len;
}

char str_get(const str *s, i64 index) { return s->data[index % s->len]; }

str cstr(const char *data) {
	str s;
	u64 data_len = strlen_helper(data);
	s.data		 = malloc(data_len * sizeof(char));
	mem_copy(s.data, data, data_len);
	s.len = data_len;
	return s;
}

void str_print(const str *s) {
	write(1, s->data, s->len);
	write(1, "\n", 1);
}

bool str_invalid(const str *s) { return s == NULL || s->data == NULL || s->len == 0; }

str str_copy(const str *restrict s) {
	if (str_invalid(s)) {
		abort();
	}
	str new = (str){malloc(s->len * sizeof(char)), s->len};
	mem_copy(new.data, s->data, s->len);
	return new;
}

i64 str_cmp(const str *restrict s1, const str *restrict s2) {
	if (str_invalid(s1) || str_invalid(s2)) {
		abort();
	}
	u64 min = s1->len < s2->len ? s1->len : s2->len;

	for (u64 i = 0; i < min; i++) {
		unsigned char a = (unsigned char)s1->data[i];
		unsigned char b = (unsigned char)s2->data[i];
		if (a != b) return a - b;
	}

	return s1->len - s2->len;
}

bool str_eq(const str *restrict s1, const str *restrict s2) {
	if (str_invalid(s1) || str_invalid(s2)) {
		abort();
	}
	if (s1->len != s2->len) return false;
	for (u64 i = 0; i < s1->len; i++) {
		if (s1->data[i] != s2->data[i]) return false;
	}
	return true;
}

u64 str_hash(const str *s) {
	/* basic djb2 hash */
	if (str_invalid(s)) return 0;
	u64 hash = 5381;
	for (u64 i = 0; i < s->len; i++) {
		hash = ((hash << 5) + hash) + s->data[i];
	}
	return hash;
}

u64 str_count_char(const str *s, char c) {
	if (str_invalid(s)) {
		return 0;
	}
	u64 count = 0;
	for (u64 i = 0; i < s->len; i++) {
		if (s->data[i] == c) count++;
	}
	return count;
}

static inline bool is_space_helper(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }

void trim_left(str *s) {
	if (str_invalid(s)) return;
	char *start = s->data;
	char *end	= s->data + s->len - 1;
	while (start <= end && is_space_helper(*start)) start++;

	u64 new_len = (end >= start) ? (end - start + 1) : 0;
	s->len		= new_len;
	if (start != s->data) {
		mem_copy(s->data, start, s->len);
	}

	char *new_data = realloc(s->data, new_len);
	if (new_data) {
		s->data = new_data;
	}
}

void trim_right(str *s) {
	if (str_invalid(s)) return;
	char *start = s->data;
	char *end	= s->data + s->len - 1;
	while (end >= start && is_space_helper(*end)) end--;

	u64 new_len = (end >= start) ? (end - start + 1) : 0;
	s->len		= new_len;
	if (start != s->data) {
		mem_copy(s->data, start, s->len);
	}

	char *new_data = realloc(s->data, new_len);
	if (new_data) {
		s->data = new_data;
	}
}

void trim(str *s) {
	if (str_invalid(s)) return;
	char *start = s->data;
	char *end	= s->data + s->len - 1;
	while (start <= end && is_space_helper(*start)) start++;
	while (end >= start && is_space_helper(*end)) end--;

	u64 new_len = (end >= start) ? (end - start + 1) : 0;

	s->len = new_len;
	if (start != s->data) {
		mem_copy(s->data, start, s->len);
	}

	char *new_data = realloc(s->data, new_len);
	if (new_data) {
		s->data = new_data;
	}
}

void ascii_to_upper(str *s) {
	if (str_invalid(s)) {
		// TODO: add error handling
		abort();
	}
	for (u64 i = 0; i < s->len; ++i) {
		if (s->data[i] >= 'a' && s->data[i] <= 'z') {
			s->data[i] -= 'a' - 'A';
		}
	}
}

void ascii_to_lower(str *s) {
	if (str_invalid(s)) {
		// TODO: add error handling
		abort();
	}
	for (u64 i = 0; i < s->len; ++i) {
		if (s->data[i] >= 'A' && s->data[i] <= 'Z') {
			s->data[i] += 'a' - 'A';
		}
	}
}

str str_reverse(const str *s) {
	if (str_invalid(s)) {
		// TODO: add error handling
		abort();
	}
	str_builder sb = str_builder_new_none();
	for (u64 i = 0; i < s->len; i++) {
		str_builder_append_char(&sb, str_get(s, s->len - i - 1));
	}
	return str_build(&sb);
}

#define min(x, y, z) ((x) < (y) ? ((x) < (z) ? (x) : (z)) : ((y) < (z) ? (y) : (z)))

i64 str_lev_distance(const str *s1, const str *s2) {
	if (str_invalid(s1) || str_invalid(s2)) {
		// TODO: add error handling
		abort();
	}

	u64 len1 = s1->len;
	u64 len2 = s2->len;

	i64 mat[len1 + 1][len2 + 1];

	for (u64 i = 0; i <= len1; ++i) {
		mat[i][0] = i;
	}
	for (u64 i = 0; i <= len2; ++i) {
		mat[0][i] = i;
	}

	for (u64 i = 1; i <= len1; ++i) {
		for (u64 j = 0; j <= len2; ++j) {
			if (str_get(s1, i - 1) == str_get(s2, j - 1)) {
				mat[i][j] = mat[i - 1][j - 1];
			} else {
				mat[i][j] = min(mat[i - 1][j], mat[i][j - 1], mat[i - 1][j - 1]) + 1;
			}
		}
	}

	return mat[len1][len2];
}

#define max(x, y) ((x) > (y) ? (x) : (y))
u64 str_count_cstr(const str *s, const char *cstr) {
	if (str_invalid(s)) {
		return 0;
	}
	u64 pattern_length = strlen_helper(cstr);

	u64 pettern_table[pattern_length];

	for (u64 i = 0; i < pattern_length; ++i) {
		pettern_table[i] = max(1, pattern_length - i);
	}

	return 0;
}
