#include <stdio.h>
#include <unistd.h>

#include <tarsio.h>
#include "backlog.h"
#include "blcheck_data.h"

#define m tarsio_mock

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

test(main_shall_return_1_if_argument_count_is_off) {
  int retval;

  mute_stderr();
  retval = main(1, NULL);
  unmute_stderr();

  assert_eq(1, retval);

  mute_stderr();
  retval = main(3, NULL);
  unmute_stderr();

  assert_eq(1, retval);
}

test(main_shall_call_backlog_read_with_correct_arguments) {
  char* argv[] = {"program_name_here", "foo.org"};
  const int make_sure_function_call_retval_is_returned = 12;
  m.backlog_read.retval = 12;
  assert_eq(make_sure_function_call_retval_is_returned, main(2, argv));
  assert_eq(0, strcmp(m.backlog_read.args.arg0, "foo.org"));
  assert_eq(NULL, m.backlog_read.args.arg1);
}
