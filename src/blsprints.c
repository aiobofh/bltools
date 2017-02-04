/*
 * List all sprint-ID's defined in the sprint list file provided to this
 * tool.
 */
#include <stdio.h>
#include <string.h>

#include "sprint.h"

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
      sprint_t sprint;
      sprint_init(&sprint, buf);
      printf("%s\n", sprint.id);
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

int main(int argc, char* argv[]) {
  /* TODO: Make nicer argument handling */
  if (2 != argc) {
    fprintf(stderr, "ERROR: One arguments requred\n");
    return 1;
  }

  const char* sprintfile = argv[1];

  sprints_read(sprintfile);

}
