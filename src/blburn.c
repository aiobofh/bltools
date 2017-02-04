/*
 * This tool will produce burn-down chart data for every sprint by matching
 * the DONE stories STARTDATE and DEADLINE dates to the provided sprint-
 * list files start and stop dates.
 */
#include <stdio.h>
#include <string.h>

#include "backlog.h"
#include "story.h"
#include "sprint.h"

#define MAX_STORIES_PER_SPRINT 100

int m_sprint_idx;
sprint_t m_sprint[2048];

static int sprints_read(const char* sprintfile) {
  int retval = 0;

  int row = 0;
  FILE* fd = call(fopen(sprintfile, "r"));
  while (!call(feof(fd))) {
    char buf[1024];
    char* b = call(fgets(buf, sizeof(buf), fd));
    if (NULL == b) {
      if (call(feof(fd))) {
        break;
      }
      fprintf(stderr, "ERROR: Unable to read file %s:%d\n", sprintfile, row);
      retval = 1;
      break;
    }
    /* Trunk linefeed if any */
    char len = strlen(buf) - 1;
    while (('\n' == buf[len]) ||
           ('\r' == buf[len])) {
      buf[len] = '\0';
      len--;
    }
    if (1 == call(is_sprint(buf))) {
      sprint_init(&m_sprint[m_sprint_idx++], buf);
    }
    else {
      fprintf(stderr, "ERROR: No sprint at %s:%d\n%s\n", sprintfile, row, buf);
      retval = 2;
      break;
    }
    row++;
  }
  call(fclose(fd));
  return retval;
}

static int sprints_cleanup() {
  while (0 < m_sprint_idx) {
    sprint_cleanup(&m_sprint[m_sprint_idx--]);
  }
}

int m_story_idx;
story_t m_story[2048];

static int append_story(story_t* story) {
  if (STATUS_DONE == story->status) {
    m_story[m_story_idx++] = *story;
  }
  return 1;
}

static sprint_t* select_sprint(const char* sprintid) {
  int sprint_idx = 0;
  while (sprint_idx < m_sprint_idx) {
    if (0 == strcmp(m_sprint[sprint_idx].id, sprintid)) {
      return &m_sprint[sprint_idx];
    }
    sprint_idx++;
  }
  return NULL;
}

static int select_stories(story_t* story[MAX_STORIES_PER_SPRINT], sprint_t* sprint) {
  int story_cnt = 0;
  int story_idx = 0;

  const long sprint_start = date2long(&sprint->start);
  const long sprint_end = date2long(&sprint->end);

  while (story_idx < m_story_idx) {
    if (STATUS_DONE != m_story[story_idx].status) {
      story_idx++;
      continue;
    }

    long story_end = date2long(&m_story[story_idx].started);

    if ((story_end >= sprint_start) && (story_end <= sprint_end)) {
      story[story_cnt++] = &m_story[story_idx];
      if (MAX_STORIES_PER_SPRINT < story_cnt) {
        fprintf(stderr, "ERROR: Too many stories done during requested sprint\n");
        return 0;
      }
    }
    story_idx++;
  }
  return story_cnt;
}

static void print_burndown(sprint_t* sprint, story_t* story[MAX_STORIES_PER_SPRINT],
                           int story_cnt) {
  long cur_date = date2long(&sprint->start);
  long end_date = date2long(&sprint->end);
  int commitment = sprint->commitment;
  while (cur_date < end_date) {
    date_t c;
    int story_idx = 0;

    while (story_idx < story_cnt) {
      story_t* s = story[story_idx];
      const long ended = date2long(&s->ended);
      if (ended == cur_date) {
        commitment -= s->estimate.points;
      }
      story_idx++;
    }

    printf("%d\n", commitment);

    long2date(&c, cur_date);
    dateadd(&c);
    cur_date = date2long(&c);
    /* TODO: Skip non-sprint-days */
  }
}

int main(int argc, char* argv[]) {
  int retval = 0;

  /* TODO: Make nicer argument handling */
  if (4 != argc) {
    fprintf(stderr, "ERROR: Three arguments requred\n");
    return 1;
  }

  const char* orgfile = argv[1];
  const char* sprintfile = argv[2];
  const char* sprintid = argv[3];

  m_story_idx = 0;

  sprints_read(sprintfile);
  backlog_read(orgfile, append_story);

  sprint_t* sprint = select_sprint(sprintid);
  if (NULL == sprint) {
    fprintf(stderr, "ERROR: Unable to find sprint-ID '%s' in %s.\n",
            sprintid, sprintfile);
    return 1;
  }

  story_t* story[MAX_STORIES_PER_SPRINT];
  const int story_cnt = select_stories(story, sprint);
  if (0 == story_cnt) {
    /* No storries found in the specified sprint */
    return 0;
  }

  print_burndown(sprint, story, story_cnt);

  return retval;
}