#include <stdio.h>
#include <unistd.h>

#include "cutest.h"

static int muted_stderr_fd;
static fpos_t stderr_pos;

extern int fileno(FILE *stream);

static void mute_stderr() {
  fflush(stderr);
  fgetpos(stdout, &stderr_pos);
  muted_stderr_fd = dup(fileno(stderr));
  freopen("/dev/null", "w", stderr);
}

static void unmute_stderr() {
  fflush(stderr);
  dup2(muted_stderr_fd, fileno(stderr));
  close(muted_stderr_fd);
  clearerr(stderr);
  fsetpos(stderr, &stderr_pos);
}

test(backlog_read_shall_return_2_if_the_file_can_not_be_opened) {
  cutest_mock.fopen.retval = NULL;
  mute_stderr();
  int retval = backlog_read("bogusfile", NULL);
  unmute_stderr();
  assert_eq(2, retval);
}

test(backlog_read_shall_open_the_file_specified_as_argument_for_reading) {
  cutest_mock.fopen.retval = (FILE*)1;
  cutest_mock.feof.retval = 1;
  int retval = backlog_read("bogusfile", NULL);
  assert_eq(0, retval);
  assert_eq(0 == strcmp(cutest_mock.fopen.args.arg0, "bogusfile"));
  assert_eq(0 == strcmp(cutest_mock.fopen.args.arg1, "r"));
}

test(backlog_read_shall_close_the_file_specified_as_argument_for_reading) {
  cutest_mock.fopen.retval = (FILE*)123;
  cutest_mock.feof.retval = 1;
  int retval = backlog_read("bogusfile", NULL);
  assert_eq(0, retval);
  assert_eq((FILE*)123, cutest_mock.fclose.args.arg0);
}

int fgets_calls;
char *fgets_stub(char *s, int size, FILE *stream) {
  fgets_calls++;
  s = s;
  size = size;
  stream = stream;
  if (fgets_calls == 4) {
    return NULL;
  }
  return "FOOO";
}

int feof_stub(FILE *stream) {
  stream = stream;
  if (fgets_calls == 4) {
    return 1;
  }
  return 0;
}

test(backlog_read_shall_fgets_every_row_of_the_file) {
  fgets_calls = 0;
  cutest_mock.fopen.retval = (FILE*)1;
  cutest_mock.fgets.func = fgets_stub;
  cutest_mock.feof.func = feof_stub;
  int retval = backlog_read("bogusfile", NULL);
  assert_eq(0, retval);
  assert_eq(4, fgets_calls);
  fgets_calls = 0;
}

test(backlog_read_shall_return_3_if_fgets_fails) {
  cutest_mock.fopen.retval = (FILE*)1;
  cutest_mock.fgets.retval = (char*)NULL;
  mute_stderr();
  int retval = backlog_read("bogusfile", NULL);
  unmute_stderr();
  assert_eq(3, retval);
}

char *fgets_stub_malformed(char *s, int size, FILE *stream) {
  size = size;
  stream = stream;
  s[0] = '*';
  s[1] = ' ';
  s[2] = 'F';
  fgets_calls++;
  return s;
}

int feof_stub_malformed(FILE *stream) {
  stream = stream;
  if (fgets_calls == 1) {
    return 1;
  }
  return 0;
}

test(backlog_read_shall_return_4_any_row_malformed) {
  fgets_calls = 0;
  cutest_mock.fopen.retval = (FILE*)1;
  cutest_mock.fgets.func = fgets_stub_malformed;
  cutest_mock.feof.func = feof_stub_malformed;
  cutest_mock.is_story.retval = 0;
  mute_stderr();
  int retval = backlog_read("bogusfile", NULL);
  unmute_stderr();
  assert_eq(4, retval);
  fgets_calls = 0;
}

char *fgets_stub_correct(char *s, int size, FILE *stream) {
  size = size;
  stream = stream;
  strcpy(s, "* TODO 02-03");
  fgets_calls++;
  return s;
}

int feof_stub_correct(FILE *stream) {
  stream = stream;
  if (fgets_calls == 1) {
    return 1;
  }
  return 0;
}

test(backlog_read_shall_return_0_no_row_is_malformed) {
  fgets_calls = 0;
  cutest_mock.fopen.retval = (FILE*)1;
  cutest_mock.fgets.func = fgets_stub_correct;
  cutest_mock.feof.func = feof_stub_correct;
  cutest_mock.is_story.retval = 1;
  int retval = backlog_read("bogusfile", NULL);
  assert_eq(0, retval);
  fgets_calls = 0;
}

int callback_calls;
story_t* callback_story;
const int weird_enough_constant = 1337;

int callback_function(story_t* story) {
  callback_calls++;
  callback_story = story;
  return 0;
}

void story_init_stub(int story, story_t* s, const char* str) {
  story = story;
  str = str;
  s->estimate.points = weird_enough_constant;
}

test(backlog_read_shall_call_the_provided_callback_function_if_row_is_ok) {
  fgets_calls = 0;
  callback_calls = 0;
  callback_story = 0;

  cutest_mock.fopen.retval = (FILE*)1;
  cutest_mock.fgets.func = fgets_stub_correct;
  cutest_mock.feof.func = feof_stub_correct;
  cutest_mock.is_story.retval = 1;
  cutest_mock.story_init.func = story_init_stub;

  int retval = backlog_read("bogusfile", callback_function);

  assert_eq(0, retval);
  assert_eq(1, callback_calls);
  assert_eq(weird_enough_constant, callback_story->estimate.points);

  callback_story = 0;
  callback_calls = 0;
  fgets_calls = 0;
}