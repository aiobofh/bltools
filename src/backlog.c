#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "backlog.h"

static int interval_read(char* dst, FILE* fd) {
  fpos_t fd_pos;
  char buf[2048];
  call(fgetpos(fd, &fd_pos));
  char* b = call(fgets(buf, sizeof(buf), fd));
  if (NULL != b) {
    call(strcpy(dst, b));
  }
  call(fsetpos(fd, &fd_pos));
  return 0;
}

int backlog_read(const char* filename, story_row_cb_t story_row_cb) {
  FILE* fd = call(fopen(filename, "r"));

  if (NULL == fd) {
    fprintf(stderr, "ERROR: Unable to open file %s\n", filename);
    return 2;
  }

  int row = 0;
  int retval = 0;

  while (0 == call(feof(fd))) {
    char buf1[2048];
    char *b1 = call(fgets(buf1, sizeof(buf1), fd));
    if (NULL == b1) {
      if (call(feof(fd))) {
        break;
      }
      fprintf(stderr, "ERROR: Unable to read file %s:%d\n", filename, row);
      retval = 3;
      break;
    }
    else if (('*' == buf1[0]) && (' ' == buf1[1])) {
      if (0 == call(is_story(buf1))) {
        fprintf(stderr,
                "ERROR: %s:%d: Expected a correctly formatted story:\n"
                "%s", filename, row, buf1);
        retval = 4;
        break;
      }
      else {
        story_t story;
        char buf2[2048];
        buf2[0] = 0;
        call(interval_read(buf2, fd));
        call(story_init(1, &story, buf1, buf2));
        if (STATUS_DONE == story.status) {
          if (ESTIMATE_RANGE == story.estimate_type) {
            fprintf(stderr,
                    "ERROR: %s:%d: Expected a correctly formatted DONE "
                    "story, estimate ranges not allowed on done stories:\n"
                    "%s\n", filename, row, buf1);
            retval = 5;
            break;
          }
          if (0 == story.started.year) {
            fprintf(stderr,
                    "ERROR: %s:%d: Expected a correctly formatted DONE "
                    "story, missing interval defined by "
                    "STARTDATE and DEADLINE on first story-row:\n"
                    "%s\n%s\n", filename, row, buf1, buf2);
            retval = 6;
            break;
          }
        }
        if (NULL != story_row_cb) {
          story_row_cb(&story);
        }
      }
    }
    row++;
  }
  call(fclose(fd));
  return retval;
}
