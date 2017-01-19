#ifndef _STORY_H_
#define _STORY_H_

typedef enum {
  STATUS_TODO,
  STATUS_DONE
} status_t;

typedef struct {
  int year;
  int month;
  int day;
} date_t;

typedef enum {
  ESTIMATE_POINTS,
  ESTIMATE_RANGE
} estimate_type_t;

typedef struct {
  status_t status;
  date_t started;
  date_t ended;
  estimate_type_t estimate_type;
  union {
    struct {
      int min_points;
      int max_points;
    } range;
    int points;
  } estimate;
  char *slogan;
  int tag_cnt;
  int* tags;
} story_t;

int is_story(const char* str);

#endif
