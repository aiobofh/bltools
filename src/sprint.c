#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "date.h"
#include "sprint.h"

static int has_date(const char* s) {
  assert(NULL != s && "NULL input not supported by has_date()");

  size_t pos = 0;

  if (0 == call(is_date(s))) {
    return 0;
  }

  pos += 10; /* strlen("yyyy-mm-dd"); */

  /* Always followed by a space */
  if (s[pos] != ' ') {
    return 0;
  }

  return 1;
}

static int has_dates(const char* str) {
  assert(NULL != str && "NULL input not supported by has_dates()");

  const int start_date = call(has_date(str));

  if (0 == start_date) {
    return 0;
  }

  const int end_date =
    call(has_date((char*)str + 10 /* strlen("yyyy-mm-dd ") - 1 */));

  if (0 == end_date) {
    return 0;
  }

  return 1;
}

static int has_title(const char* str) {
  assert(NULL != str && "NULL input not supported by has_title()");

  if (22 >= strlen(str)) { /* 2 * strlen("yyyy-mm-dd ") */
    return 0;
  }

  return 1;
}

static void get_date(date_t* d, const char* s) {
  d->year = (((s[0] - '0') * 1000) +
             ((s[1] - '0') * 100) +
             ((s[2] - '0') * 10) +
             ((s[3] - '0')));
  d->month = (((s[5] - '0') * 10) + (s[6] - '0'));
  d->day = (((s[8] - '0') * 10) + (s[9] - '0'));
}

static void get_start_date(date_t* start, const char* str) {
  call(get_date(start, str));
}

static void get_end_date(date_t* start, const char* str) {
  call(get_date(start, (char*)str + 11)); /* strlen("yyyy-mm-dd ") */
}

static int get_title_length(const char* str) {
  const char *s = &str[21];
  return strlen(s);
}

static void get_title(char* dest, size_t len, const char* str) {
  const char *s = &str[21];
  strncpy(dest, s, len);
}
