#ifndef _DATE_H_
#define _DATE_H_

typedef struct {
  int year;
  int month;
  int day;
} date_t;

int is_date(const char* str);

#endif
