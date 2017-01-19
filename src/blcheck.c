/*
 * This tool will just parse the file given as first argument, and then
 * return 0 on success or 1 on failure, along with error messages on what
 * rows are not correctly formatted.
 */

#include <stdlib.h>
#include <stdio.h>

#include "story.h"

int quiet = 0;

int blcheck(int argc, char* argv[]) {
  if (2 != argc) {
    if (0 == quiet) {
      fprintf(stderr, "ERROR: One argument requred\n");
    }
    return 1;
  }

  /* TODO: Make nicer argument handling */

  char *filename = argv[1];
  FILE* fd = call(fopen(filename, "r"));

  if (NULL == fd) {
    if (0 == quiet) {
      fprintf(stderr, "ERROR: Unable to open file %s\n", filename);
    }
    return 2;
  }

  int row = 0;
  int retval = 0;

  while (0 == call(feof(fd))) {
    char buf[2048];
    if (NULL == call(fgets(buf, sizeof(buf), fd))) {
      if (call(feof(fd))) {
        break;
      }
      if (0 == quiet) {
        fprintf(stderr, "ERROR: Unable to read file %s:%d\n", filename, row);
      }
      retval = 3;
      break;
    }
    else if (('*' == buf[0]) && (0 == call(is_story(buf)))) {
      if (0 == quiet) {
        fprintf(stderr,
                "ERROR: %s:%d: Expected a correctly formatted story:\n"
                "%s", filename, row, buf);
      }
      retval = 4;
    }
    row++;
  }
  call(fclose(fd));
  return retval;
}

int main(int argc, char* argv[]) {
  return blcheck(argc, argv);
}
