#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#if defined(__clang__) || defined(__GNUC__)

/* Int types */
typedef __INT8_TYPE__  i8;
typedef __UINT8_TYPE__ u8;

typedef __INT16_TYPE__	i16;
typedef __UINT16_TYPE__ u16;

typedef __INT32_TYPE__	i32;
typedef __UINT32_TYPE__ u32;

typedef __INT64_TYPE__	i64;
typedef __UINT64_TYPE__ u64;

typedef float	f32;
typedef double f64;

typedef unsigned char byte;
typedef unsigned char uchar;

typedef __SIZE_TYPE__	 size_t;
typedef __PTRDIFF_TYPE__ ptrdiff;
typedef __INTPTR_TYPE__	 iptr;
typedef __UINTPTR_TYPE__ uptr;

#define NULL ((void*)0)

#else
#error "Compiler not yet supported for types.h."
#endif

#endif
