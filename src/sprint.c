/*
 * Parser functions for reading a sprint-list file on the format:
 *
 * yyyy-mm-dd yyyy-mm-dd ID-of-the-first-sprint
 * yyyy-mm-dd yyyy-mm-dd ID-of-the-second-sprint
 *
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "date.h"
#include "sprint.h"

#define COMMITMENTPOS 22
#define IDPOS 25

/*
 * Check if the input string is formatted as a date, followed by a space
 */
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

/*
 * Check if the input string starts with two consecutive date strings.
 */
static int has_dates(const char* str) {
  assert(NULL != str && "NULL input not supported by has_dates()");

  const int start_date = call(has_date(str));

  if (0 == start_date) {
    return 0;
  }

  const int end_date =
    call(has_date((char*)str + 11 /* strlen("yyyy-mm-dd ") - 1 */));

  if (0 == end_date) {
    return 0;
  }

  return 1;
}

/*
 * Check if the input string has a two-digit commitment after the
 * dates.
 */
static int has_commitment(const char* str) {
  assert(NULL != str && "NULL input not supported by has_commitment()");

  if (COMMITMENTPOS >= strlen(str)) { /* 2 * strlen("yyyy-mm-dd ") */
    return 0;
  }

  const char *s = &str[COMMITMENTPOS];

  if (((s[0] < '0') || (s[0] > '9')) ||
      ((s[1] < '0') || (s[1] > '9')) ||
      ((s[2] != ' '))) {
    return 0;
  }

  return 1;
}

/*
 * Check if the input string has a conitnuation after a specifig position
 * this is considered to be a sprint id.
 */
static int has_id(const char* str) {
  assert(NULL != str && "NULL input not supported by has_id()");

  if (IDPOS >= strlen(str)) { /* 2 * strlen("yyyy-mm-dd ") + 3 */
    return 0;
  }

  size_t i;
  const char *s = &str[IDPOS];
  for (i = 0; i < strlen(s); i++) {
    if (((s[i] < '0') || (s[i] > '9')) &&
        ((s[i] < 'A') || (s[i] > 'Z')) &&
        ((s[i] < 'a') || (s[i] > 'z')) &&
        ((s[i] != '-') && (s[i] != '_'))) {
      return 0;
    }
  }

  return 1;
}

/*
 * Transform the input string to a date_t destination.
 */
static void get_date(date_t* d, const char* s) {
  assert(NULL != s && "NULL input string not supporterd by get_date()");
  assert(NULL != d && "NULL input date not supporterd by get_date()");

  d->year = (((s[0] - '0') * 1000) +
             ((s[1] - '0') * 100) +
             ((s[2] - '0') * 10) +
             ((s[3] - '0')));
  d->month = (((s[5] - '0') * 10) + (s[6] - '0'));
  d->day = (((s[8] - '0') * 10) + (s[9] - '0'));
}

/*
 * Transform the first date in the input string to the start date varibale.
 */
static void get_start_date(date_t* start, const char* str) {
  assert(NULL != start && "NULL start date not supported by get_start_date()");
  assert(NULL != str && "NULL string not supported by get_start_date()");
  assert(1 == has_dates(str) && "Input string never verified to have dates");

  call(get_date(start, str));
}

/*
 * Transform the second date in the input string to the start date varibale.
 */
static void get_end_date(date_t* start, const char* str) {
  assert(NULL != start && "NULL end date not supported by get_start_date()");
  assert(NULL != str && "NULL string not supported by get_start_date()");
  assert(1 == has_dates(str) && "Input string never verified to have dates");

  call(get_date(start, (char*)str + 11)); /* strlen("yyyy-mm-dd ") */
}

/*
 * Get the commitment in points for the sprint in the input string.
 */
static int get_commitment(const char* str) {
  assert(NULL != str && "NULL string not supported by get_commitment()");
  assert(1 == has_commitment(str) && "Input string never verified to have commitment");

  return ((str[COMMITMENTPOS + 0] - '0') * 10 + (str[COMMITMENTPOS + 1] - '0'));
}

/*
 * Count the characters from after the two dates to the end of the string,
 * stripping away white-spaces and such.
 */
static size_t get_id_length(const char* str) {
  assert(NULL != str && "NULL string not supported by get_id_length()");
  assert(1 == has_id(str) && "Input string never verified to have id");

  const char *s = &str[IDPOS];

  assert(s[0] != ' ' && "IDPOS is a bad assumption");

  int len = strlen(s) - 1;

  /* Trim the end of the string */

  /* TODO: Implement/use a rtrim function */
  while (('\n' == s[len]) ||
         ('\r' == s[len]) ||
         (' ' == s[len]) ||
         ('\t' == s[len])) {
    len--;
  }
  return len + 1;
}

/*
 * Copy the id of a sprint to the pre-allocated destination string.
 */
static void get_id(char* dest, size_t len, const char* str) {
  assert(NULL != dest && "NULL destinatino not supported by get_id()");
  assert(NULL != str && "NULL string not supported by get_id()");
  assert(len == get_id_length(str) && "Length never verified");

  const char *s = &str[IDPOS];
  memcpy(dest, s, len);
  dest[len] = '\0';
}

/*
 * Check if the given input string looks like a sprint definition.
 */
int is_sprint(const char* str) {
  assert(NULL != str && "NULL string not supported by is_sprint()");

  if ((0 == call(has_dates(str))) ||
      (0 == call(has_commitment(str))) ||
      (0 == call(has_id(str)))) {
    return 0;
  }

  return 1;
}

/*
 * Initialize a sprint_t variable form the input string.
 */
void sprint_init(sprint_t* sprint, const char* str) {
  assert(NULL != sprint && "sprint must be allocated");
  assert(NULL != str && "A file row must be passed as argument");
  assert(1 == is_sprint(str) && "Input string never verified");

  const int dates = call(has_dates(str));
  const int commitment = call(has_commitment(str));
  const int id = call(has_id(str));

  if ((0 == dates) || (0 == id) || (0 == commitment)) {
    return;
  }

  const int length = call(get_id_length(str));

  if (0 == length) {
    return;
  }

  const int commitment_estimate = call(get_commitment(str));

  call(get_start_date(&sprint->start, str));
  call(get_end_date(&sprint->end, str));

  sprint->commitment = commitment_estimate;

  sprint->id = call(malloc(length + 1)); /* Space for NULL termination */
  assert(NULL != sprint->id && "Out of memory");
  call(get_id(sprint->id, length, str));
}

void sprint_cleanup(sprint_t* sprint) {
  assert(NULL != sprint && "NULL sprint not supported by sprint_cleanup()");
  assert(NULL != sprint->id && "Sprint ID must have been allocated");
  call(free(sprint->id));
}
