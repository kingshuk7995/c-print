#include "c_print.h"

int main(void) {
  int x = 42;
  double y = 3.14159;
  const char *s = "Hello, C11!";
  char c = 'A';
  long long big = 123456789012345LL;
  unsigned u = 99;
  float f = 2.71828f;

  c_print("x = {}\n", x);
  c_print("x = {}, y = {}, s = {}, c = {}\n", x, y, s, c);
  c_print("big={}, u={}, f={}, unnamed = {}\n", big, u, f, 0.234);
  // escaped braces:
  c_print("literal braces: {{ and }} and value {}\n", x);

  return 0;
}
