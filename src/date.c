#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <strings.h>

#include "date.h"

int is_date(const char* s) {
  assert(NULL != s && "NULL input string not uspported by is_date()");
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

long date2long(date_t* date) {
  assert(NULL != date && "NULL date is not supported by date2long");

  char buf[12];
  /* TODO: Remove need for sprintf, is possible */
  sprintf(buf, "%04d%02d%02d", date->year, date->month, date->day);
  return atol(buf);
}

void long2date(date_t* dst, long int date) {
  dst->year = date / 10000;
  dst->month = date / 100 - dst->year * 100;
  dst->day = date - dst->year * 10000 - dst->month * 100;
}

void dateadd(date_t* date) {
  time_t timestamp;
  struct tm tm;
  struct tm* tmp;
  bzero(&tm, sizeof(struct tm));
  tm.tm_sec = 0;
  tm.tm_min = 0;
  tm.tm_hour = 12;
  tm.tm_mday = date->day;
  tm.tm_mon = date->month - 1;
  tm.tm_year = date->year - 1900;
  timestamp = mktime(&tm);
  timestamp += 60 * 60 * 24;
  tmp = gmtime(&timestamp);
  date->year = tmp->tm_year + 1900;
  date->month = tmp->tm_mon + 1;
  date->day = tmp->tm_mday;
}
