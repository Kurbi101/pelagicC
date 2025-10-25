#ifndef TO_STRING_H
#define TO_STRING_H

#include "../string.h"

str bool_to_str(bool b);

str char_to_str(char c);

str i8_to_str_decimal(i8 num);
str i16_to_str_decimal(i16 num);
str i32_to_str_decimal(i32 num);
str i64_to_str_decimal(i64 num);

str u8_to_str_decimal(u8 num);
str u16_to_str_decimal(u16 num);
str u32_to_str_decimal(u32 num);
str u64_to_str_decimal(u64 num);

str i8_to_str_hex(i8 num);
str i16_to_str_hex(i16 num);
str i32_to_str_hex(i32 num);
str i64_to_str_hex(i64 num);

str u8_to_str_hex(u8 num);
str u16_to_str_hex(u16 num);
str u32_to_str_hex(u32 num);
str u64_to_str_hex(u64 num);
str ptr_to_str(void *p);

str i8_to_str_bin(i8 num);
str i16_to_str_bin(i16 num);
str i32_to_str_bin(i32 num);
str i64_to_str_bin(i64 num);
str u8_to_str_bin(u8 num);
str u16_to_str_bin(u16 num);
str u32_to_str_bin(u32 num);
str u64_to_str_bin(u64 num);

str f32_to_str(f32 num, u8 precision);
str f64_to_str(f64 num, u8 precision);

str i8_dec_formatter(void *num);
str i64_dec_formatter(void *num);

#endif
