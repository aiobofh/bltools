#include <stdio.h>

#include "date.h"

int is_date(const char* s) {
  size_t pos;

  /* Four digit year */
  for (pos = 0; pos < 4; pos++) {
    if (('0' > s[pos]) || ('9' < s[pos])) {
      return 0;
    }
  }

  if ('-' != s[4]) {
    return 0;
  }

  /* Two digit month */
  for (pos = 5; pos < 5 + 2; pos++) {
    if (('0' > s[pos]) || ('9' < s[pos])) {
      return 0;
    }
  }

  if ('-' != s[7]) {
    return 0;
  }

  /* Two digit day */
  for (pos = 8; pos < 8 + 2; pos++) {
    if (('0' > s[pos]) || ('9' < s[pos])) {
      return 0;
    }
  }

  return 1;
}
