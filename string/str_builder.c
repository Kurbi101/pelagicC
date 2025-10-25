#include "../mem.h"
#include "../string.h"

static inline u64 strlen_helper(const char *restrict data) {
	u64 len = 0;
	while (*data++) len++;
	return len;
}

str_builder str_builder_new_none(void) {
	str_builder sb;
	sb.is_built = false;
	sb.cap		= STR_BUILDER_CAP;
	sb.data		= malloc(sizeof(char) * sb.cap);
	sb.len		= 0;
	return sb;
}
str_builder str_builder_new_cstr(const char *cstr) {
	str_builder sb;
	sb.is_built = false;
	sb.cap		= STR_BUILDER_CAP;
	sb.len		= strlen_helper(cstr);
	while (sb.len > sb.cap) {
		sb.cap *= 2;
	}
	sb.data = malloc(sizeof(char) * sb.cap);
	sb.data = mem_copy(sb.data, cstr, sb.len);
	return sb;
}
str_builder str_builder_new_str(const str *s) {
	str_builder sb;
	sb.is_built = false;
	sb.cap		= STR_BUILDER_CAP;
	sb.len		= s->len;
	while (sb.len > sb.cap) {
		sb.cap *= 2;
	}
	sb.data = malloc(sizeof(char) * sb.cap);
	sb.data = mem_copy(sb.data, s->data, sb.len);
	return sb;
}

str_builder str_builder_new_char(char c) {
	str_builder sb;
	sb.is_built = false;
	sb.cap		= STR_BUILDER_CAP;
	sb.len		= 1;
	sb.data		= malloc(sizeof(char) * sb.cap);
	sb.data[0]	= c;
	return sb;
}

str_builder str_builder_append_cstr(str_builder *builder, const char *cstr) {
	if (builder->is_built) {
		// TODO: add error handling
		return *builder;
	}
	u64 cstr_len = strlen_helper(cstr);
	if (builder->cap <= builder->len + cstr_len) {
		while (builder->cap <= builder->len + cstr_len) {
			builder->cap *= 2;
		}
		builder->data = realloc(builder->data, builder->cap);
	}
	mem_copy(&builder->data[builder->len], cstr, cstr_len);
	builder->len += cstr_len;
	return *builder;
}

str_builder str_builder_append_str(str_builder *builder, str *s) {
	if (builder->is_built) {
		// TODO: add error handling
		return *builder;
	}
	if (builder->cap <= builder->len + s->len) {
		while (builder->cap <= builder->len + s->len) {
			builder->cap *= 2;
		}
		builder->data = realloc(builder->data, builder->cap);
	}
	mem_copy(&builder->data[builder->len], s->data, s->len);
	builder->len += s->len;
	return *builder;
}

str_builder str_builder_append_char(str_builder *builder, char c) {
	if (builder->is_built) {
		// TODO: add error handling
		return *builder;
	}
	if (builder->cap <= builder->len + 1) {
		builder->cap *= 2;
		builder->data = realloc(builder->data, builder->cap);
	}
	builder->data[builder->len++] = c;
	return *builder;
}

str_builder str_builder_append_int(str_builder *builder, int i) { return str_builder_append_char(builder, (char)i); }

str str_build(str_builder *builder) {
	builder->is_built = true;
	str result;
	result.len	= builder->len;
	result.data = realloc(builder->data, result.len);
	return result;
}

void str_builder_cleanup(str_builder *sb) {
	free(sb->data);
	sb->data = NULL;
	sb->cap	 = 0;
	sb->len	 = 0;
}

void str_builder_pop(str_builder *sb) {
	if (sb->len > 0) {
		sb->len--;
	} else {
		// TODO: add error handling
		abort();
	}
}

void str_builder_reverse(str_builder *sb) {
	for (u64 i = 0; i < sb->len / 2; i++) {
		char temp				  = sb->data[i];
		sb->data[i]				  = sb->data[sb->len - i - 1];
		sb->data[sb->len - i - 1] = temp;
	}
}
