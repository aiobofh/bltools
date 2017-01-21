/*
 * This tool will just parse the file given as first argument, and then
 * return 0 on success or 1 on failure, along with error messages on what
 * rows are not correctly formatted.
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "backlog.h"
#include "story.h"

int sum;
story_row_cb_t func;

int done(story_t* story) {
  if (STATUS_DONE == story->status) {
    call(sum_min_estimates(&sum, story));
  }
  return 0;
}

int todo_min(story_t* story) {
  if (STATUS_TODO == story->status) {
    call(sum_min_estimates(&sum, story));
  }
  return 0;
}

int todo_max(story_t* story) {
  if (STATUS_TODO == story->status) {
    call(sum_max_estimates(&sum, story));
  }
  return 0;
}

int main(int argc, char* argv[]) {
  int retval = 0;

  /* TODO: Make nicer argument handling */
  if ((4 != argc) && (3 != argc)) {
    fprintf(stderr, "ERROR: Two to three argument requred\n");
    return 1;
  }

  char* filename;

  if (('-' == argv[1][0]) && ('d' == argv[1][1])) {
    func = done;
    filename = argv[2];
  }
  else if (('-' == argv[1][0]) && ('t' == argv[1][1])) {
    if (('-' == argv[2][0]) && ('x' == argv[2][1])) {
      func = todo_max;
    }
    else if (('-' == argv[2][0]) && ('n' == argv[2][1])) {
      func = todo_min;
    }
    else {
      fprintf(stderr, "ERROR: -t need either -x (max) or -n (min)\n");
      return 1;
    }
    filename = argv[3];
  }
  else {
    fprintf(stderr, "ERROR: Unknown option -%c\n", argv[1][1]);
    return 1;
  }

  assert(NULL != func);

  retval = call(backlog_read(filename, func));

  printf("%d\n", sum);

  return retval;
}
