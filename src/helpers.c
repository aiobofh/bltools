#include "helpers.h"

void rtrim(char* buf) {
  int len = strlen(buf) - 1;
  while ((len >= 0) && (0 != isspace(buf[len]))) {
    len--;
  }
  buf[len + 1] = 0;
}
