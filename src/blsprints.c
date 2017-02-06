/*
 * List all sprint-ID's defined in the sprint list file provided to this
 * tool.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "sprint.h"

#define BUFSIZE 512

static char* get_row(char* dst, FILE* fd, const char* sprintfile, int row) {
  char* b = call(fgets(dst, BUFSIZE, fd));
  if (NULL == b) {
    if (call(feof(fd))) {
      return NULL;
    }
    fprintf(stderr, "ERROR: Unable to read file %s:%d\n", sprintfile, row);
    return NULL;
  }
  return b;
}

static void rtrim(char* buf) {
  int len = strlen(buf) - 1;
  while ((len >= 0) && (0 != isspace(buf[len]))) {
    len--;
  }
  buf[len + 1] = 0;
}

static void print_sprint(const char* buf) {
  sprint_t sprint;
  call(sprint_init(&sprint, buf));
  call(fputs(sprint.id, stdout));
  call(putc('\n', stdout));
  call(sprint_cleanup(&sprint));
}

static int parse_and_print(FILE *fd, const char* sprintfile, int row) {
  char buf[BUFSIZE];
  char* b;
  if (NULL == (b = call(get_row(buf, fd, sprintfile, row)))) {
    return 1;
  }
  call(rtrim(buf));
  if (0 != call(is_sprint(buf))) {
    call(print_sprint(buf));
  }
  else {
    fprintf(stderr, "ERROR: No sprint %s:%d\n%s\n", sprintfile, row, buf);
    return 2;
  }
  return 0;
}

static int sprints_read(const char* sprintfile) {
  int row = 0;
  FILE* fd = call(fopen(sprintfile, "r"));
  if (NULL == fd) {
    fprintf(stderr, "ERROR: Could not open '%s' for reading\n", sprintfile);
    return 1;
  }
  while (!call(feof(fd))) {
    if (0 != call(parse_and_print(fd, sprintfile, row))) {
      call(fclose(fd));
      return 2;
    }
    row++;
  }
  call(fclose(fd));
  return 0;
}

int main(int argc, char* argv[]) {
  /* TODO: Make nicer argument handling */
  if (2 != argc) {
    fprintf(stderr, "ERROR: One arguments requred\n");
    return EXIT_FAILURE;
  }

  const char* sprintfile = argv[1];

  if (0 != call(sprints_read(sprintfile))) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
