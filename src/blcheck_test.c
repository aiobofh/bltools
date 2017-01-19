#include "cutest.h"

extern int quiet;

test(blcheck_shall_return_1_if_argument_count_is_off) {
  quiet = 1;
  assert_eq(1, blcheck(1, NULL));
  assert_eq(1, blcheck(3, NULL));
}

test(blcheck_shall_return_2_if_the_file_can_not_be_opened) {
  char* argv[] = {"blcheck", "foo.org"};
  quiet = 1;
  cutest_mock.fopen.retval = NULL;
  assert_eq(2, blcheck(2, argv));
}

test(blcheck_shall_open_the_file_specified_as_argument_for_reading) {
  char* argv[] = {"blcheck", "foo.org"};
  quiet = 1;
  cutest_mock.fopen.retval = (FILE*)1;
  cutest_mock.feof.retval = 1;
  assert_eq(0, blcheck(2, argv));
  assert_eq(0 == strcmp(cutest_mock.fopen.args.arg0, "foo.org"));
  assert_eq(0 == strcmp(cutest_mock.fopen.args.arg1, "r"));
}

test(blcheck_shall_close_the_file_specified_as_argument_for_reading) {
  char* argv[] = {"blcheck", "foo.org"};
  quiet = 1;
  cutest_mock.fopen.retval = (FILE*)123;
  cutest_mock.feof.retval = 1;
  assert_eq(0, blcheck(2, argv));
  assert_eq((FILE*)123, cutest_mock.fclose.args.arg0);
}

int fgets_calls;
char *fgets_stub(char *s, int size, FILE *stream) {
  fgets_calls++;
  s = s;
  size = size;
  stream = stream;
  return "FOOO";
}

int feof_stub(FILE *stream) {
  stream = stream;
  if (fgets_calls == 4) {
    return 1;
  }
  return 0;
}

test(blcheck_shall_fgets_every_row_of_the_file) {
  char* argv[] = {"blcheck", "foo.org"};
  quiet = 1;
  fgets_calls = 0;
  cutest_mock.fopen.retval = (FILE*)1;
  cutest_mock.fgets.func = fgets_stub;
  cutest_mock.feof.func = feof_stub;
  assert_eq(0, blcheck(2, argv));
  assert_eq(4, fgets_calls);
  fgets_calls = 0;
}

test(blcheck_shall_return_3_if_fgets_fails) {
  char* argv[] = {"blcheck", "foo.org"};
  quiet = 1;
  cutest_mock.fopen.retval = (FILE*)1;
  cutest_mock.fgets.retval = (char*)NULL;
  assert_eq(3, blcheck(2, argv));
}

char *fgets_stub_malformed(char *s, int size, FILE *stream) {
  size = size;
  stream = stream;
  s[0] = '*';
  s[1] = 'F';
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

test(blcheck_shall_return_4_any_row_malformed) {
  char* argv[] = {"blcheck", "foo.org"};
  quiet = 1;
  fgets_calls = 0;
  cutest_mock.fopen.retval = (FILE*)1;
  cutest_mock.fgets.func = fgets_stub_malformed;
  cutest_mock.feof.func = feof_stub_malformed;
  cutest_mock.is_story.retval = 0;
  assert_eq(4, blcheck(2, argv));
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

test(blcheck_shall_return_0_no_row_is_malformed) {
  char* argv[] = {"blcheck", "foo.org"};
  quiet = 1;
  fgets_calls = 0;
  cutest_mock.fopen.retval = (FILE*)1;
  cutest_mock.fgets.func = fgets_stub_correct;
  cutest_mock.feof.func = feof_stub_correct;
  cutest_mock.is_story.retval = 1;
  assert_eq(0, blcheck(2, argv));
  fgets_calls = 0;
}
