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
  char* b = fgets(dst, BUFSIZE, fd);
  if (NULL == b) {
    if (feof(fd)) {
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
  sprint_init(&sprint, buf);
  fputs(sprint.id, stdout);
  putc('\n', stdout);
  sprint_cleanup(&sprint);
}

static int parse_and_print(FILE *fd, const char* sprintfile, int row) {
  char buf[BUFSIZE];
  char* b;
  if (NULL == (b = get_row(buf, fd, sprintfile, row))) {
    return 1;
  }
  rtrim(buf);
  if (0 != is_sprint(buf)) {
    print_sprint(buf);
  }
  else {
    fprintf(stderr, "ERROR: No sprint %s:%d\n%s\n", sprintfile, row, buf);
    return 2;
  }
  return 0;
}

static int sprints_simple_read(const char* sprintfile) {
  int row = 0;
  FILE* fd = fopen(sprintfile, "r");
  if (NULL == fd) {
    fprintf(stderr, "ERROR: Could not open '%s' for reading\n", sprintfile);
    return 1;
  }
  while (!feof(fd)) {
    if (0 != parse_and_print(fd, sprintfile, row)) {
      fclose(fd);
      return 2;
    }
    row++;
  }
  fclose(fd);
  return 0;
}

int main(int argc, char* argv[]) {
  /* TODO: Make nicer argument handling */
  if (2 != argc) {
    fprintf(stderr, "ERROR: One arguments requred\n");
    return EXIT_FAILURE;
  }

  const char* sprintfile = argv[1];

  if (0 != sprints_simple_read(sprintfile)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
