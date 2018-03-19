#include <stdio.h>
#include <unistd.h>

#include "cutest.h"

#define mock cutest_mock(NAME) cutest_mock.NAME

test(interval_read_shall_get_the_current_position_of_the_file_correctly) {
  interval_read((char*)1234, (FILE*)5678); /* Function to test */

  assert_eq(1, cutest_mock.fgetpos.call_count);
  assert_eq((FILE*)5678, cutest_mock.fgetpos.args.arg0);
}

test(interval_read_shall_get_one_line_from_the_file) {
  interval_read((char*)1234, (FILE*)5678); /* Function to test */

  assert_eq(1, cutest_mock.fgets.call_count);
  assert_eq((FILE*)5678, cutest_mock.fgets.args.arg2);
}

test(interval_read_shall_copy_the_file_row_to_dst_if_file_could_be_read) {
  cutest_mock.fgets.retval = (char*)4321;

  interval_read((char*)1234, (FILE*)5678); /* Function to test */

  assert_eq(1, cutest_mock.strcpy.call_count);
  assert_eq((char*)1234, cutest_mock.strcpy.args.arg0);
  assert_eq((char*)4321, cutest_mock.strcpy.args.arg1);
}

test(interval_read_shall_reset_the_file_position_to_the_one_read_earlier) {
  interval_read((char*)1234, (FILE*)5678); /* Function to test */

  assert_eq(1, cutest_mock.fsetpos.call_count);
  assert_eq((FILE*)5678, cutest_mock.fsetpos.args.arg0);
  assert_eq(cutest_mock.fgetpos.args.arg1, cutest_mock.fsetpos.args.arg1);
}

test(backlog_read_shall_return_2_if_the_file_can_not_be_opened) {
  cutest_mock.fopen.retval = NULL;

  int retval = backlog_read("bogusfile", NULL); /* Function to test */

  assert_eq(2, retval);
}

test(backlog_read_shall_open_the_file_specified_as_argument_for_reading) {
  cutest_mock.fopen.retval = (FILE*)1;
  cutest_mock.feof.retval = 1;

  int retval = backlog_read("bogusfile", NULL); /* Function to test */

  assert_eq(0, retval);
  assert_eq(0 == strcmp(cutest_mock.fopen.args.arg0, "bogusfile"));
  assert_eq(0 == strcmp(cutest_mock.fopen.args.arg1, "r"));
}

test(backlog_read_shall_close_the_file_specified_as_argument_for_reading) {
  cutest_mock.fopen.retval = (FILE*)123;
  cutest_mock.feof.retval = 1;

  int retval = backlog_read("bogusfile", NULL); /* Function to test */

  assert_eq(0, retval);
  assert_eq((FILE*)123, cutest_mock.fclose.args.arg0);
}

int fgets_calls;
char *fgets_stub(char *s, int size, FILE *stream) {
  fgets_calls++;
  (void)(s = s);
  (void)(size = size);
  (void)(stream = stream);
  if (fgets_calls == 4) {
    return NULL;
  }
  return "FOOO";
}

int feof_stub(FILE *stream) {
  (void)(stream = stream);
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

  int retval = backlog_read("bogusfile", NULL); /* Function to test */

  assert_eq(0, retval);
  assert_eq(4, fgets_calls);
  fgets_calls = 0;
}

test(backlog_read_shall_return_3_if_fgets_fails) {
  cutest_mock.fopen.retval = (FILE*)1;
  cutest_mock.fgets.retval = (char*)NULL;

  int retval = backlog_read("bogusfile", NULL); /* Function to test */

  assert_eq(3, retval);
}

char *fgets_stub_malformed(char *s, int size, FILE *stream) {
  (void)(size = size);
  (void)(stream = stream);
  s[0] = '*'; s[1] = ' '; s[2] = 'F';
  fgets_calls++;
  return s;
}

int feof_stub_malformed(FILE *stream) {
  (void)(stream = stream);
  if (fgets_calls == 1) {
    return 1;
  }
  return 0;
}

test(backlog_read_shall_return_4_if_any_bullet_row_is_malformed) {
  fgets_calls = 0;
  cutest_mock.fopen.retval = (FILE*)1;
  cutest_mock.fgets.func = fgets_stub_malformed;
  cutest_mock.feof.func = feof_stub_malformed;
  cutest_mock.is_story.retval = 0;

  int retval = backlog_read("bogusfile", NULL); /* Function to test */

  assert_eq(4, retval);
  fgets_calls = 0;
}

char *fgets_stub_incorrect_done(char *s, int size, FILE *stream) {
  (void)(size = size);
  (void)(stream = stream);
  strcpy(s, "* DONE 02-03");
  fgets_calls++;
  return s;
}

char *fgets_stub_correct(char *s, int size, FILE *stream) {
  (void)(size = size);
  (void)(stream = stream);
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

#define STORY_INIT_UNUSED_ARGS                  \
  (void)(story = story);                        \
  (void)(str1 = str1);                          \
  (void)(str2 = str2)

#define STORY_INIT_STUB_ARGS                    \
  STORY_INIT_UNUSED_ARGS;                       \
  s->status = STATUS_DONE;                      \
  s->estimate_type = ESTIMATE_POINTS

void story_init_stub_with_done_and_range(int story, story_t* s,
                                         const char* str1,
                                         const char* str2) {
  STORY_INIT_UNUSED_ARGS;
  s->status = STATUS_DONE;
  s->estimate_type = ESTIMATE_RANGE;
}

test(backlog_read_shall_return_5_if_a_done_story_has_an_estimate_range) {
  fgets_calls = 0;
  cutest_mock.fopen.retval = (FILE*)1;
  cutest_mock.fgets.func = fgets_stub_incorrect_done;
  cutest_mock.feof.func = feof_stub_correct;
  cutest_mock.is_story.retval = 1;
  cutest_mock.story_init.func = story_init_stub_with_done_and_range;

  int retval = backlog_read("bogusfile", NULL); /* Function to test */

  assert_eq(5, retval);
  fgets_calls = 0;
}

#define BACKLOG_READ_DONE_INTERVAL_FIXTURE             \
  fgets_calls = 0;                                      \
  cutest_mock.fopen.retval = (FILE*)1;                  \
  cutest_mock.fgets.func = fgets_stub_incorrect_done;   \
  cutest_mock.feof.func = feof_stub_correct;            \
  cutest_mock.is_story.retval = 1

void story_init_stub_with_done_and_no_interval(int story, story_t* s,
                                               const char* str1,
                                               const char* str2) {
  STORY_INIT_STUB_ARGS;
  s->started.year = 0;
}

test(backlog_read_shall_return_6_if_a_done_story_has_no_interval) {
  BACKLOG_READ_DONE_INTERVAL_FIXTURE;
  cutest_mock.story_init.func = story_init_stub_with_done_and_no_interval;

  int retval = backlog_read("bogusfile", NULL); /* Function to test */

  assert_eq(6, retval);
  fgets_calls = 0;
}

void story_init_stub_with_done_and_has_interval(int story, story_t* s,
                                               const char* str1,
                                               const char* str2) {
  STORY_INIT_STUB_ARGS;
  s->started.year = 2017;
}

test(backlog_read_shall_return_0_if_a_done_story_has_interval) {
  BACKLOG_READ_DONE_INTERVAL_FIXTURE;
  cutest_mock.story_init.func = story_init_stub_with_done_and_has_interval;

  int retval = backlog_read("bogusfile", NULL); /* Function to test */

  assert_eq(0, retval);
  fgets_calls = 0;
}

#undef BACKLOG_READ_DONE_INTERVAL_FIXTURE

void story_init_stub_with_todo(int story, story_t* s,
                               const char* str1,
                               const char* str2) {
  STORY_INIT_UNUSED_ARGS;
  s->status = STATUS_TODO;
  s->estimate_type = ESTIMATE_POINTS;
}

#define BACKLOG_READ_NORMAL_OPERATIONS_FIXTURE \
  fgets_calls = 0;                             \
  cutest_mock.fopen.retval = (FILE*)1;         \
  cutest_mock.fgets.func = fgets_stub_correct; \
  cutest_mock.feof.func = feof_stub_correct;   \
  cutest_mock.is_story.retval = 1

test(backlog_read_shall_return_0_no_row_is_malformed) {
  BACKLOG_READ_NORMAL_OPERATIONS_FIXTURE;
  cutest_mock.story_init.func = story_init_stub_with_todo;

  int retval = backlog_read("bogusfile", NULL); /* Function to test */

  assert_eq(0, retval);
  fgets_calls = 0;
}

int callback_calls;
story_t* callback_story;
const int weird_enough_constant = 1337;

int cb_function(story_t* story) {
  callback_calls++;
  callback_story = story;
  return 0;
}

void story_init_stub(int story, story_t* s, const char* str1,
                     const char* str2) {
  STORY_INIT_UNUSED_ARGS;
  s->estimate.points = weird_enough_constant;
}

#undef STORY_INIT_UNUSED_ARGS

test(backlog_read_shall_call_the_provided_callback_function_if_row_is_ok) {
  BACKLOG_READ_NORMAL_OPERATIONS_FIXTURE;

  cutest_mock.story_init.func = story_init_stub;

  callback_calls = 0;
  callback_story = 0;

  int r = backlog_read("bogusfile", cb_function); /* Function to test */

  assert_eq(0, r);
  assert_eq(1, callback_calls);
  assert_eq(weird_enough_constant, callback_story->estimate.points);

  callback_story = 0;
  callback_calls = 0;
  fgets_calls = 0;
}

#undef BACKLOG_READ_NORMAL_OPERATIONS_FIXTURE
