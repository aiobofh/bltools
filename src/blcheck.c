/*
 * This tool will just parse the file given as first argument, and then
 * return 0 on success or 1 on failure, along with error messages on what
 * rows are not correctly formatted.
 */

#include <stdlib.h>
#include <stdio.h>

#include "backlog.h"

int main(int argc, char* argv[]) {
  /* TODO: Make nicer argument handling */
  if (2 != argc) {
    fprintf(stderr, "ERROR: One argument requred\n");
    return 1;
  }

  return backlog_read(argv[1], NULL);
}
