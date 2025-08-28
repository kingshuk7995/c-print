#ifndef C_PRINT_H
#define C_PRINT_H

// c_print.h - simple single-header type-safe "{}" formatter for C11+ like
// C++20's std::format Usage: c_print("x = {}", x);
// Supports up to 12 arguments by default (easy to increase).

#include <inttypes.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

// --- type tags ---
enum cp_type {
  CP_INT,
  CP_LONG,
  CP_LONGLONG,
  CP_UINT,
  CP_ULONG,
  CP_ULONGLONG,
  CP_FLOAT,
  CP_DOUBLE,
  CP_LDOUBLE,
  CP_CHAR,
  CP_CSTR,
  CP_PTR
};

// Map C expression -> cp_type
#define CP_CODE(x)                      \
  _Generic((x),                         \
      int: CP_INT,                      \
      long: CP_LONG,                    \
      long long: CP_LONGLONG,           \
      unsigned: CP_UINT,                \
      unsigned long: CP_ULONG,          \
      unsigned long long: CP_ULONGLONG, \
      float: CP_FLOAT,                  \
      double: CP_DOUBLE,                \
      long double: CP_LDOUBLE,          \
      char: CP_CHAR,                    \
      char *: CP_CSTR,                  \
      const char *: CP_CSTR,            \
      default: CP_PTR)

// --- helper: count macro args (0..12) ---
#define PP_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, N, ...) N
#define PP_RSEQ_N() 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)
#define PP_NARG(...) PP_NARG_(__VA_ARGS__, PP_RSEQ_N())

// --- pair helper macros: produce (typecode, value) pairs for each arg ---
// up to 12 params (extend in need).
#define CP_PAIR1(a1) CP_CODE(a1), (a1)
#define CP_PAIR2(a1, a2) CP_PAIR1(a1), CP_PAIR1(a2)
#define CP_PAIR3(a1, a2, a3) CP_PAIR2(a1, a2), CP_PAIR1(a3)
#define CP_PAIR4(a1, a2, a3, a4) CP_PAIR3(a1, a2, a3), CP_PAIR1(a4)
#define CP_PAIR5(a1, a2, a3, a4, a5) CP_PAIR4(a1, a2, a3, a4), CP_PAIR1(a5)
#define CP_PAIR6(a1, a2, a3, a4, a5, a6) \
  CP_PAIR5(a1, a2, a3, a4, a5), CP_PAIR1(a6)
#define CP_PAIR7(a1, a2, a3, a4, a5, a6, a7) \
  CP_PAIR6(a1, a2, a3, a4, a5, a6), CP_PAIR1(a7)
#define CP_PAIR8(a1, a2, a3, a4, a5, a6, a7, a8) \
  CP_PAIR7(a1, a2, a3, a4, a5, a6, a7), CP_PAIR1(a8)
#define CP_PAIR9(a1, a2, a3, a4, a5, a6, a7, a8, a9) \
  CP_PAIR8(a1, a2, a3, a4, a5, a6, a7, a8), CP_PAIR1(a9)
#define CP_PAIR10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
  CP_PAIR9(a1, a2, a3, a4, a5, a6, a7, a8, a9), CP_PAIR1(a10)
#define CP_PAIR11(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
  CP_PAIR10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10), CP_PAIR1(a11)
#define CP_PAIR12(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
  CP_PAIR11(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11), CP_PAIR1(a12)

// --- overload selectors for each count ---
#define c_print_0(fmt) _c_print_dispatch((fmt), 0)
#define c_print_1(fmt, a1) _c_print_dispatch((fmt), 1, CP_PAIR1(a1))
#define c_print_2(fmt, a1, a2) _c_print_dispatch((fmt), 2, CP_PAIR2(a1, a2))
#define c_print_3(fmt, a1, a2, a3) \
  _c_print_dispatch((fmt), 3, CP_PAIR3(a1, a2, a3))
#define c_print_4(fmt, a1, a2, a3, a4) \
  _c_print_dispatch((fmt), 4, CP_PAIR4(a1, a2, a3, a4))
#define c_print_5(fmt, a1, a2, a3, a4, a5) \
  _c_print_dispatch((fmt), 5, CP_PAIR5(a1, a2, a3, a4, a5))
#define c_print_6(fmt, a1, a2, a3, a4, a5, a6) \
  _c_print_dispatch((fmt), 6, CP_PAIR6(a1, a2, a3, a4, a5, a6))
#define c_print_7(fmt, a1, a2, a3, a4, a5, a6, a7) \
  _c_print_dispatch((fmt), 7, CP_PAIR7(a1, a2, a3, a4, a5, a6, a7))
#define c_print_8(fmt, a1, a2, a3, a4, a5, a6, a7, a8) \
  _c_print_dispatch((fmt), 8, CP_PAIR8(a1, a2, a3, a4, a5, a6, a7, a8))
#define c_print_9(fmt, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
  _c_print_dispatch((fmt), 9, CP_PAIR9(a1, a2, a3, a4, a5, a6, a7, a8, a9))
#define c_print_10(fmt, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
  _c_print_dispatch((fmt), 10,                                   \
                    CP_PAIR10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10))
#define c_print_11(fmt, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
  _c_print_dispatch((fmt), 11,                                        \
                    CP_PAIR11(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11))
#define c_print_12(fmt, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) _c_print_dispatch((fmt), 12, CP_PAIR12(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12)

// main dispatching macro (choose correct overload)
#define CAT(a, b) a##b
#define SELECT_COUNT_MACRO(fmt, cnt, ...) CAT(c_print_, cnt)(fmt, __VA_ARGS__)
#define c_print(fmt, ...) \
  SELECT_COUNT_MACRO(fmt, PP_NARG(__VA_ARGS__), __VA_ARGS__)

// --- Implementation: parse format and consume (typecode,value) pairs from
// varargs --- This function is static inline so header-only usage won't produce
// duplicate symbols.
static inline void _c_print_dispatch(const char *fmt, int n, ...) {
  va_list ap;
  va_start(ap, n);

  const char *p = fmt;
  int used = 0;

  while (*p) {
    if (p[0] == '{' && p[1] == '{') {  // escaped {
      putchar('{');
      p += 2;
      continue;
    }
    if (p[0] == '}' && p[1] == '}') {  // escaped }
      putchar('}');
      p += 2;
      continue;
    }
    if (p[0] == '{' && p[1] == '}') {
      if (used >= n) {
        // Not enough arguments provided: print placeholder and continue
        fputs("[ERR:missing arg]", stdout);
        p += 2;
        continue;
      }
      // read the pair: int code, then value (type depends on code)
      int code = va_arg(ap, int);
      switch (code) {
        case CP_INT: {
          int v = va_arg(ap, int);
          printf("%d", v);
          break;
        }
        case CP_LONG: {
          long v = va_arg(ap, long);
          printf("%ld", v);
          break;
        }
        case CP_LONGLONG: {
          long long v = va_arg(ap, long long);
          printf("%lld", v);
          break;
        }
        case CP_UINT: {
          unsigned v = va_arg(ap, unsigned);
          printf("%u", v);
          break;
        }
        case CP_ULONG: {
          unsigned long v = va_arg(ap, unsigned long);
          printf("%lu", v);
          break;
        }
        case CP_ULONGLONG: {
          unsigned long long v = va_arg(ap, unsigned long long);
          printf("%llu", v);
          break;
        }
        case CP_FLOAT: {  // promoted to double
          double v = va_arg(ap, double);
          printf("%f", v);
          break;
        }
        case CP_DOUBLE: {
          double v = va_arg(ap, double);
          printf("%f", v);
          break;
        }
        case CP_LDOUBLE: {
          long double v = va_arg(ap, long double);
          printf("%Lf", v);
          break;
        }
        case CP_CHAR: {  // promoted to int
          int v = va_arg(ap, int);
          printf("%c", (char)v);
          break;
        }
        case CP_CSTR: {
          const char *v = va_arg(ap, const char *);
          if (v)
            printf("%s", v);
          else
            printf("(null)");
          break;
        }
        case CP_PTR:
        default: {
          void *v = va_arg(ap, void *);
          printf("%p", v);
          break;
        }
      }
      used++;
      p += 2;
      continue;
    }
    // normal character
    putchar(*p++);
  }

  va_end(ap);
}

#endif  // C_PRINT_H
