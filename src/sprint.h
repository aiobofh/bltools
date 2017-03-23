#ifndef _SPRINT_H_
#define _SPRINT_H_

#include "date.h"

typedef struct {
  date_t start;
  date_t end;
  int schedule_cnt;
  date_t* schedule;
  int commitment;
  char* id;
} sprint_t;

typedef int (*sprint_row_cb_t)(const sprint_t* story);

int is_sprint(const char* str);
void sprint_init(sprint_t* sprint, const char* str);
void sprint_cleanup(sprint_t* sprint);
int sprints_read(const char* filename, sprint_row_cb_t sprint_row_cb);

#endif
