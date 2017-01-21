#include <stdio.h>
#include <stdlib.h>

#include "backlog.h"

int backlog_read(const char* filename, story_row_cb_t story_row_cb) {
  FILE* fd = call(fopen(filename, "r"));

  if (NULL == fd) {
    fprintf(stderr, "ERROR: Unable to open file %s\n", filename);
    return 2;
  }

  int row = 0;
  int retval = 0;

  while (0 == call(feof(fd))) {
    char buf[2048];
    char *b = call(fgets(buf, sizeof(buf), fd));
    if (NULL == b) {
      if (call(feof(fd))) {
        break;
      }
      fprintf(stderr, "ERROR: Unable to read file %s:%d\n", filename, row);
      retval = 3;
      break;
    }
    else if (('*' == buf[0]) && (' ' == buf[1])) {
      if (0 == call(is_story(buf))) {
        fprintf(stderr,
                "ERROR: %s:%d: Expected a correctly formatted story:\n"
                "%s", filename, row, buf);
        retval = 4;
      }
      else {
        if (NULL != story_row_cb) {
          story_t story;
          call(story_init(1, &story, buf));
          story_row_cb(&story);
        }
      }
    }
    row++;
  }
  call(fclose(fd));
  return retval;
}
