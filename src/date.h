#ifndef _DATE_H_
#define _DATE_H_

typedef struct {
  int year;
  int month;
  int day;
} date_t;

int is_date(const char* str);
long date2long(date_t* date);
void long2date(date_t* dst, long int date);
void dateadd(date_t* date);
void date2shortname(char dst[3], date_t* date);

#endif
