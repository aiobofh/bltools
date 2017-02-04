#ifndef _SPRINT_H_
#define _SPRINT_H_

#include "date.h"

typedef struct {
  date_t start;
  date_t end;
  int commitment;
  char* id;
} sprint_t;

int is_sprint(const char* str);
void sprint_init(sprint_t* sprint, const char* str);
void sprint_cleanup(sprint_t* sprint);

#endif
