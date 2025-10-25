#include "to_string.h"
#include "../string.h"
#include "../types.h"

#define SIGNED_DECIMAL_TO_STR                                                                                          \
	str_builder sb			= str_builder_new_none();                                                                  \
	bool		is_negative = num < 0;                                                                                 \
                                                                                                                       \
	while (num > 0) {                                                                                                  \
		str_builder_append(&sb, (char)('0' + num % 10));                                                               \
		num /= 10;                                                                                                     \
	}                                                                                                                  \
                                                                                                                       \
	if (is_negative) {                                                                                                 \
		str_builder_append(&sb, (char)'-');                                                                            \
	}                                                                                                                  \
                                                                                                                       \
	str_builder_reverse(&sb);                                                                                          \
	return str_build(&sb);

#define UNSIGNED_DECIMAL_TO_STR                                                                                        \
	str_builder sb = str_builder_new_none();                                                                           \
                                                                                                                       \
	while (num > 0) {                                                                                                  \
		str_builder_append(&sb, (char)('0' + num % 10));                                                               \
		num /= 10;                                                                                                     \
	}                                                                                                                  \
                                                                                                                       \
	str_builder_reverse(&sb);                                                                                          \
	return str_build(&sb);

#define SIGNED_HEX_TO_STR                                                                                              \
	bool		is_negative = num < 0;                                                                                 \
	str_builder sb			= str_builder_new_none();                                                                  \
	u64			unsigned_num;                                                                                          \
                                                                                                                       \
	if (is_negative) {                                                                                                 \
		unsigned_num = (u64)(-(num + 1)) + 1;                                                                          \
	} else {                                                                                                           \
		unsigned_num = (u64)num;                                                                                       \
	}                                                                                                                  \
                                                                                                                       \
	if (unsigned_num == 0) {                                                                                           \
		str_builder_append(&sb, (char)'0');                                                                            \
	} else {                                                                                                           \
		while (unsigned_num > 0) {                                                                                     \
			u8 digit = unsigned_num % 16;                                                                              \
			if (digit < 10) {                                                                                          \
				str_builder_append(&sb, (char)('0' + digit));                                                          \
			} else {                                                                                                   \
				str_builder_append(&sb, (char)('A' + digit - 10));                                                     \
			}                                                                                                          \
			unsigned_num /= 16;                                                                                        \
		}                                                                                                              \
	}                                                                                                                  \
	str_builder_append(&sb, is_negative ? "x0-" : "x0");                                                               \
	str_builder_reverse(&sb);                                                                                          \
	return str_build(&sb);

#define UNSIGNED_HEX_TO_STR                                                                                            \
	str_builder sb = str_builder_new_none();                                                                           \
                                                                                                                       \
	if (num == 0) {                                                                                                    \
		str_builder_append(&sb, (char)'0');                                                                            \
	} else {                                                                                                           \
		while (num > 0) {                                                                                              \
			int digit = num % 16;                                                                                      \
			if (digit < 10) {                                                                                          \
				str_builder_append(&sb, (char)('0' + digit));                                                          \
			} else {                                                                                                   \
				str_builder_append(&sb, (char)('A' + digit - 10));                                                     \
			}                                                                                                          \
			num /= 16;                                                                                                 \
		}                                                                                                              \
	}                                                                                                                  \
	str_builder_append(&sb, "x0");                                                                                     \
	str_builder_reverse(&sb);                                                                                          \
	return str_build(&sb);

#define SIGNED_BIN_TO_STR                                                                                              \
	bool		is_negative = num < 0;                                                                                 \
	str_builder sb			= str_builder_new_none();                                                                  \
	u64			unsigned_num;                                                                                          \
                                                                                                                       \
	if (is_negative) {                                                                                                 \
		unsigned_num = (u64)(-(num + 1)) + 1;                                                                          \
	} else {                                                                                                           \
		unsigned_num = (u64)num;                                                                                       \
	}                                                                                                                  \
                                                                                                                       \
	if (unsigned_num == 0) {                                                                                           \
		str_builder_append(&sb, (char)'0');                                                                            \
	} else {                                                                                                           \
		while (unsigned_num > 0) {                                                                                     \
			u8 bit = unsigned_num & 1;                                                                                 \
			str_builder_append(&sb, (char)('0' + bit));                                                                \
			unsigned_num >>= 1;                                                                                        \
		}                                                                                                              \
	}                                                                                                                  \
	str_builder_append(&sb, is_negative ? "b0-" : "b0");                                                               \
	str_builder_reverse(&sb);                                                                                          \
	return str_build(&sb);

#define UNSIGNED_BIN_TO_STR                                                                                            \
	str_builder sb = str_builder_new_none();                                                                           \
                                                                                                                       \
	if (num == 0) {                                                                                                    \
		str_builder_append(&sb, (char)'0');                                                                            \
	} else {                                                                                                           \
		while (num > 0) {                                                                                              \
			u8 bit = num & 1;                                                                                          \
			str_builder_append(&sb, (char)('0' + bit));                                                                \
			num >>= 1;                                                                                                 \
		}                                                                                                              \
	}                                                                                                                  \
	str_builder_append(&sb, "b0");                                                                                     \
	str_builder_reverse(&sb);                                                                                          \
	return str_build(&sb);

#define FLOAT_TO_STR(precision)                                                                                        \
	str_builder sb			= str_builder_new_none();                                                                  \
	f64			value		= (f64)num;                                                                                \
	bool		is_negative = value < 0.0;                                                                             \
                                                                                                                       \
	if (is_negative) {                                                                                                 \
		value = -value;                                                                                                \
		str_builder_append(&sb, (char)'-');                                                                            \
	}                                                                                                                  \
                                                                                                                       \
	if (value != value) {                                                                                              \
		str_builder_append(&sb, "NaN");                                                                                \
		return str_build(&sb);                                                                                         \
	}                                                                                                                  \
	if (value > 1.7976931348623157e+308) {                                                                             \
		str_builder_append(&sb, "inf");                                                                                \
		return str_build(&sb);                                                                                         \
	}                                                                                                                  \
                                                                                                                       \
	u64 int_part  = (u64)value;                                                                                        \
	f64 frac_part = value - (f64)int_part;                                                                             \
                                                                                                                       \
	if (int_part == 0) {                                                                                               \
		str_builder_append(&sb, (char)'0');                                                                            \
	} else {                                                                                                           \
		str_builder sb_int = str_builder_new_none();                                                                   \
		u64			temp   = int_part;                                                                                 \
		while (temp > 0) {                                                                                             \
			str_builder_append(&sb_int, (char)('0' + temp % 10));                                                      \
			temp /= 10;                                                                                                \
		}                                                                                                              \
		str_builder_reverse(&sb_int);                                                                                  \
		str int_str = str_build(&sb_int);                                                                              \
		str_builder_append_str(&sb, &int_str);                                                                         \
	}                                                                                                                  \
                                                                                                                       \
	if (precision > 0) {                                                                                               \
		str_builder_append(&sb, (char)'.');                                                                            \
		for (int i = 0; i < precision; i++) {                                                                          \
			frac_part *= 10.0;                                                                                         \
			u8 digit = (u8)frac_part;                                                                                  \
			str_builder_append(&sb, (char)('0' + digit));                                                              \
			frac_part -= (f64)digit;                                                                                   \
		}                                                                                                              \
	}                                                                                                                  \
                                                                                                                       \
	return str_build(&sb);

str cstr_to_str(const char *cs) { return cstr(cs); }

str bool_to_str(bool b) {
	if (b) {
		return cstr("true");
	} else {
		return cstr("false");
	}
}

str char_to_str(char c) {
	char *ch = malloc(sizeof(char));
	*ch		 = c;
	return (str){ch, 1};
}

str ptr_to_str(void *p) {
	if (!p) {
		return cstr("NULL");
	}
	str_builder sb	= str_builder_new_none();
	u64			num = (u64)p;
	while (num > 0) {
		int digit = num % 16;
		if (digit < 10) {
			str_builder_append(&sb, (char)('0' + digit));
		} else {
			str_builder_append(&sb, (char)('A' + digit - 10));
		}
		num /= 16;
	}
	str_builder_append(&sb, "x0");
	str_builder_reverse(&sb);
	return str_build(&sb);
}

str ptrdiff_to_str(ptrdiff num) { UNSIGNED_HEX_TO_STR; }

str i8_to_str_decimal(i8 num) { SIGNED_DECIMAL_TO_STR; }
str i16_to_str_decimal(i16 num) { SIGNED_DECIMAL_TO_STR; }
str i32_to_str_decimal(i32 num) { SIGNED_DECIMAL_TO_STR; }
str i64_to_str_decimal(i64 num) { SIGNED_DECIMAL_TO_STR; }
str u8_to_str_decimal(u8 num) { UNSIGNED_DECIMAL_TO_STR; }
str u16_to_str_decimal(u16 num) { UNSIGNED_DECIMAL_TO_STR; }
str u32_to_str_decimal(u32 num) { UNSIGNED_DECIMAL_TO_STR; }
str u64_to_str_decimal(u64 num) { UNSIGNED_DECIMAL_TO_STR; }

str i8_to_str_hex(i8 num) { SIGNED_HEX_TO_STR; }
str i16_to_str_hex(i16 num) { SIGNED_HEX_TO_STR; }
str i32_to_str_hex(i32 num) { SIGNED_HEX_TO_STR; }
str i64_to_str_hex(i64 num) { SIGNED_HEX_TO_STR; }
str u8_to_str_hex(u8 num) { UNSIGNED_HEX_TO_STR; }
str u16_to_str_hex(u16 num) { UNSIGNED_HEX_TO_STR; }
str u32_to_str_hex(u32 num) { UNSIGNED_HEX_TO_STR; }
str u64_to_str_hex(u64 num) { UNSIGNED_HEX_TO_STR; }

str i8_to_str_bin(i8 num) { SIGNED_BIN_TO_STR; }
str i16_to_str_bin(i16 num) { SIGNED_BIN_TO_STR; }
str i32_to_str_bin(i32 num) { SIGNED_BIN_TO_STR; }
str i64_to_str_bin(i64 num) { SIGNED_BIN_TO_STR; }
str u8_to_str_bin(u8 num) { UNSIGNED_BIN_TO_STR; }
str u16_to_str_bin(u16 num) { UNSIGNED_BIN_TO_STR; }
str u32_to_str_bin(u32 num) { UNSIGNED_BIN_TO_STR; }
str u64_to_str_bin(u64 num) { UNSIGNED_BIN_TO_STR; }

str f32_to_str(f32 num, u8 precision) { FLOAT_TO_STR(precision); }
str f64_to_str(f64 num, u8 precision) { FLOAT_TO_STR(precision); }

str i8_dec_formatter(void *num) {
	i8 *ptr = (i8 *)num;
	return i8_to_str_decimal(*ptr);
}

str i64_dec_formatter(void *num) {
	i64 *ptr = (i64 *)num;
	return i64_to_str_decimal(*ptr);
}