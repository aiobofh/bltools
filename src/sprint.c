/*
 * Parser functions for reading a sprint-list file on the format:
 *
 * yyyy-mm-dd yyyy-mm-dd ID-of-the-first-sprint
 * yyyy-mm-dd yyyy-mm-dd ID-of-the-second-sprint
 *
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <strings.h>

#include "date.h"
#include "sprint.h"

#define SCHEDULEPOS 22
#define COMMITMENTPOS 22
#define IDPOS 25

/*
 * Check if the input string is formatted as a date, followed by a space
 */
static int has_date(const char* s) {
  assert(NULL != s && "NULL input not supported by has_date()");

  size_t pos = 0;

  if (0 == is_date(s)) {
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

  const int start_date = has_date(str);

  if (0 == start_date) {
    return 0;
  }

  const int end_date =
    has_date((char*)str + 11 /* strlen("yyyy-mm-dd ") - 1 */);

  if (0 == end_date) {
    return 0;
  }

  return 1;
}

static int is_weekday_short(const char* s) {
  const int retval = ((('M' == s[0]) && ('o' == s[1])) ||
                      (('T' == s[0]) && ('u' == s[1])) ||
                      (('W' == s[0]) && ('e' == s[1])) ||
                      (('T' == s[0]) && ('h' == s[1])) ||
                      (('F' == s[0]) && ('r' == s[1])) ||
                      (('S' == s[0]) && ('a' == s[1])) ||
                      (('S' == s[0]) && ('u' == s[1]))) ? 1 : 0;
  return retval;
}

/*
 * Check if the input string has a schedule (MoToWe...) after the dates
 */
static int has_schedule(const char* str) {
  assert(NULL != str && "NULL input not supported by has_schedule()");

  if (SCHEDULEPOS >= strlen(str)) { /* 2 * strlen("yyyy-mm-dd ") */
    return 0;
  }

  char *s = (char*)&str[SCHEDULEPOS];
  char *end = s + strlen(s) - 1;
  int weekday = 0;
  while ((1 == (weekday = is_weekday_short(s))) && (s < end)) {
    s += 2;
    if (' ' == s[0]) {
      break;
    }
  }

  return weekday;
}

static int skip_schedule_length(const char* str) {
  int pos = SCHEDULEPOS;
  while (str[pos] != ' ') {
    pos++;
  }
  return pos;
}

/*
 * Check if the input string has a two-digit commitment after the
 * schedule.
 */
static int has_commitment(const char* str) {
  assert(NULL != str && "NULL input not supported by has_commitment()");

  const size_t comment_pos = skip_schedule_length(str) + 1;

  if (comment_pos >= strlen(str)) { /* 2 * strlen("yyyy-mm-dd ") + X */
    return 0;
  }

  const char *s = &str[comment_pos];

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

  const size_t id_pos = skip_schedule_length(str) + 1 + 2 + 1;

  if (id_pos >= strlen(str)) { /* 2 * strlen("yyyy-mm-dd ") + X */
    return 0;
  }

  const char *s = &str[id_pos];

  int len = 0;
  while (!(('\n' == s[len]) ||
           ('\r' == s[len]) ||
           (' ' == s[len]) ||
           ('\0' == s[len]) ||
           ('\t' == s[len]))) {
    if (((s[len] < '0') || (s[len] > '9')) &&
        ((s[len] < 'A') || (s[len] > 'Z')) &&
        ((s[len] < 'a') || (s[len] > 'z')) &&
        ((s[len] != '-') && (s[len] != '_'))) {
      return 0;
    }
    len++;
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

  get_date(start, str);
}

/*
 * Transform the second date in the input string to the start date varibale.
 */
static void get_end_date(date_t* start, const char* str) {
  assert(NULL != start && "NULL end date not supported by get_start_date()");
  assert(NULL != str && "NULL string not supported by get_start_date()");
  assert(1 == has_dates(str) && "Input string never verified to have dates");

  get_date(start, (char*)str + 11); /* strlen("yyyy-mm-dd ") */
}

/*
 * Get the number of days from the schedule.
 */
static int get_days_in_schedule(const char* str) {
  char *s = (char*)&str[SCHEDULEPOS];
  char *end = s + strlen(s) - 1;
  int days = 0;
  while ((1 == is_weekday_short(s)) && (s < end)) {
    s += 2;
    days++;
    if (' ' == s[0]) {
      break;
    }
  }
  return days;
}

static int get_wday(const char* str) {
  if (('M' == str[0]) && ('o' == str[1])) {
    return 1;
  }
  if (('T' == str[0]) && ('u' == str[1])) {
    return 2;
  }
  if (('W' == str[0]) && ('e' == str[1])) {
    return 3;
  }
  if (('T' == str[0]) && ('h' == str[1])) {
    return 4;
  }
  if (('F' == str[0]) && ('r' == str[1])) {
    return 5;
  }
  if (('S' == str[0]) && ('a' == str[1])) {
    return 6;
  }
  /* Assume Su */
  return 0;
}

static void date2tm(struct tm* tm, date_t* date) {
  bzero(tm, sizeof(*tm));
  tm->tm_sec = 0;
  tm->tm_min = 0;
  tm->tm_hour = 12;
  tm->tm_mday = date->day;
  tm->tm_mon = date->month - 1;
  tm->tm_year = date->year - 1900;
}

/*
 * Fill-out the specified list of dates with dates for every day in the
 * schedule.
 */
static void get_schedule(date_t* start, date_t* date, const char* str) {
  char *s = (char*)&str[SCHEDULEPOS];
  char *end = s + strlen(s) - 1;
  int weekday = 0;
  struct tm start_tm;
  struct tm* tmp;
  time_t timestamp;
  int idx = 0;

  date2tm(&start_tm, start);
  timestamp = mktime(&start_tm);
  tmp = gmtime(&timestamp);

  while ((1 == (weekday = is_weekday_short(s))) && (s < end)) {
    int wday = get_wday(s);

    while (tmp->tm_wday != wday) {
      timestamp = mktime(tmp);
      timestamp += 60 * 60 * 24;
      tmp = gmtime(&timestamp);
    }

    date[idx].year = tmp->tm_year + 1900;
    date[idx].month = tmp->tm_mon + 1;
    date[idx].day = tmp->tm_mday;
    idx++;

    s += 2;

    if (' ' == s[0]) {
      break;
    }
  }
}

/*
 * Get the commitment in points for the sprint in the input string.
 */
static int get_commitment(const char* str) {
  assert(NULL != str && "NULL string not supported by get_commitment()");
  assert(1 == has_commitment(str) && "Input string never verified to have commitment");

  int commitment_pos = SCHEDULEPOS;
  while (str[commitment_pos] != ' ') {
    commitment_pos++;
  }
  commitment_pos++;

  return ((str[commitment_pos + 0] - '0') * 10 +
          (str[commitment_pos + 1] - '0'));
}

/*
 * Count the characters from after the two dates to the end of the string,
 * stripping away white-spaces and such.
 */
static size_t get_id_length(const char* str) {
  assert(NULL != str && "NULL string not supported by get_id_length()");
  assert(1 == has_id(str) && "Input string never verified to have id");

  int commitment_pos = SCHEDULEPOS;
  while (str[commitment_pos] != ' ') {
    commitment_pos++;
  }
  commitment_pos++;

  int id_pos = commitment_pos;
  while (str[id_pos] != ' ') {
    id_pos++;
  }
  id_pos++;

  const char *s = &str[id_pos];

  assert(s[0] != ' ' && "id_pos is a bad assumption");

  int len = 0;
  while (!(('\n' == s[len]) ||
           ('\r' == s[len]) ||
           (' ' == s[len]) ||
           ('\0' == s[len]) ||
           ('\t' == s[len]))) {
    len++;
  }
  return len;
}

/*
 * Copy the id of a sprint to the pre-allocated destination string.
 */
static void get_id(char* dest, size_t len, const char* str) {
  assert(NULL != dest && "NULL destinatino not supported by get_id()");
  assert(NULL != str && "NULL string not supported by get_id()");
  assert(len == get_id_length(str) && "Length never verified");

  int commitment_pos = SCHEDULEPOS;
  while (str[commitment_pos] != ' ') {
    commitment_pos++;
  }
  commitment_pos++;

  int id_pos = commitment_pos;
  while (str[id_pos] != ' ') {
    id_pos++;
  }
  id_pos++;

  const char *s = &str[id_pos];
  memcpy(dest, s, len);
  dest[len] = '\0';
}

/*
 * Check if the given input string looks like a sprint definition.
 */
int is_sprint(const char* str) {
  assert(NULL != str && "NULL string not supported by is_sprint()");

  if ((0 == has_dates(str)) ||
      (0 == has_schedule(str)) ||
      (0 == has_commitment(str)) ||
      (0 == has_id(str))) {
    return 0;
  }

  return 1;
}

static void init_dates(date_t* start, date_t* end, const char* str) {
  get_start_date(start, str);
  get_end_date(end, str);
}

static void init_schedule(int* cnt, date_t* start, date_t** sched,
                         const char* str) {
  const int schedule_cnt = get_days_in_schedule(str);
  *sched = malloc(sizeof(**sched) * schedule_cnt);
  assert(NULL != *sched && "Out of memory for schedule array");
  *cnt = schedule_cnt;
  get_schedule(start, *sched, str);
}

static void init_commitment(int* commitment, const char* str) {
  *commitment = get_commitment(str);
}

static void init_id(char** id, const char* str) {
  const int id_length = get_id_length(str);
  *id = malloc(id_length + 1); /* Space for NULL termination */
  assert(NULL != *id && "Out of memory for id string");
  get_id(*id, id_length, str);
}

/*
 * Initialize a sprint_t variable form the input string.
 */
void sprint_init(sprint_t* sprint, const char* str) {
  assert(NULL != sprint && "sprint must be allocated");
  assert(NULL != str && "A file row must be passed as argument");
  assert(1 == is_sprint(str) && "Input string never verified");

  init_dates(&sprint->start, &sprint->end, str);
  init_schedule(&sprint->schedule_cnt, &sprint->start,
                &sprint->schedule, str);
  init_commitment(&sprint->commitment, str);
  init_id(&sprint->id, str);
}

static void cleanup_id(char** id) {
  free(*id);
  *id = NULL;
}

static void cleanup_schedule(date_t** schedule) {
  free(*schedule);
  *schedule = NULL;
}

void sprint_cleanup(sprint_t* sprint) {
  assert(NULL != sprint && "NULL sprint not supported by sprint_cleanup()");
  assert(NULL != sprint->id && "Sprint ID must have been allocated");

  cleanup_id(&sprint->id);
  cleanup_schedule(&sprint->schedule);
}

int sprints_read(const char* filename, sprint_row_cb_t sprint_row_cb) {
  assert(NULL != filename &&
         "NULL filename is not supported by sprintes_read");
  int retval = 0;
  int row = 0;

  FILE* fd = fopen(filename, "r");
  if (NULL == fd) {
    fprintf(stderr, "ERROR: Unable to open file %s\n", filename);
    return 1;
  }
  while (!feof(fd)) {
    char buf[1024];
    char* b = fgets(buf, sizeof(buf), fd);
    if (NULL == b) {
      if (feof(fd)) {
        break;
      }
      fprintf(stderr, "ERROR: Unable to read file %s:%d\n", filename, row);
      retval = 2;
      break;
    }

    /* Trunk linefeed if any */
    size_t len = strlen(buf) - 1;
    buf[len] = '\0';

    if (1 == is_sprint(buf)) {
      sprint_t sprint;
      sprint_init(&sprint, buf);
      if (NULL != sprint_row_cb) {
        sprint_row_cb(&sprint);
      }
    }
    else {
      fprintf(stderr,
              "ERROR: No sprint at %s:%d\n%s\n", filename, row, buf);
      retval = 3;
      break;
    }
    row++;
  }
  fclose(fd);
  return retval;
}
