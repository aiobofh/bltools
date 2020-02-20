#include <stdio.h>
#include <unistd.h>

#include <tarsio.h>
#include "sprint.h"
#include "backlog.h"
#include "blsum_data.h"

#define m tarsio_mock

static int muted_stderr_fd;
static fpos_t stderr_pos;

static int muted_stdout_fd;
static fpos_t stdout_pos;

extern int sum;

extern int fileno(FILE *stream);

static void mute_stderr() {
  fflush(stderr);
  fgetpos(stderr, &stderr_pos);
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

static void mute_stdout() {
  fflush(stdout);
  fgetpos(stdout, &stdout_pos);
  muted_stdout_fd = dup(fileno(stdout));
  freopen("/dev/null", "w", stdout);
}

static void unmute_stdout() {
  fflush(stdout);
  dup2(muted_stdout_fd, fileno(stdout));
  close(muted_stdout_fd);
  clearerr(stdout);
  fsetpos(stdout, &stdout_pos);
}

test(done_shall_call_sum_min_estimates_correctly_if_status_is_done) {
  sum = 10;
  story_t story;
  story.status = STATUS_DONE;
  story.estimate.points = 20;
  done(&story);
  assert_eq(1, m.sum_min_estimates.call_count);
  assert_eq(&sum, m.sum_min_estimates.args.arg0);
  assert_eq(&story, m.sum_min_estimates.args.arg1);
}

test(done_shall_not_call_sum_min_estimates_if_status_is_todo) {
  sum = 10;
  story_t story;
  story.status = STATUS_TODO;
  story.estimate.points = 20;
  done(&story);
  assert_eq(0, m.sum_min_estimates.call_count);
}

test(todo_min_shall_call_sum_min_estimates_correctly_if_status_is_todo) {
  sum = 10;
  story_t story;
  story.status = STATUS_TODO;
  story.estimate.points = 20;
  todo_min(&story);
  assert_eq(1, m.sum_min_estimates.call_count);
  assert_eq(&sum, m.sum_min_estimates.args.arg0);
  assert_eq(&story, m.sum_min_estimates.args.arg1);
}

test(todo_min_shall_not_call_sum_min_estimates_if_status_is_done) {
  sum = 10;
  story_t story;
  story.status = STATUS_DONE;
  story.estimate.points = 20;
  todo_min(&story);
  assert_eq(0, m.sum_min_estimates.call_count);
}

test(todo_max_shall_call_sum_max_estimates_correctly_if_status_is_todo) {
  sum = 10;
  story_t story;
  story.status = STATUS_TODO;
  story.estimate.points = 20;
  todo_max(&story);
  assert_eq(1, m.sum_max_estimates.call_count);
  assert_eq(&sum, m.sum_max_estimates.args.arg0);
  assert_eq(&story, m.sum_max_estimates.args.arg1);
}

test(todo_max_shall_not_call_sum_max_estimates_if_status_is_done) {
  sum = 10;
  story_t story;
  story.status = STATUS_DONE;
  story.estimate.points = 20;
  todo_max(&story);
  assert_eq(0, m.sum_max_estimates.call_count);
}

test(main_shall_return_1_if_argument_count_is_off) {
  int retval;

  mute_stderr();
  retval = main(2, NULL);
  unmute_stderr();

  assert_eq(1, retval);

  mute_stderr();
  retval = main(5, NULL);
  unmute_stderr();

  assert_eq(1, retval);
}

test(main_shall_return_2_if_t_argument_is_not_followd_by_x_or_n) {
  int retval;
  char* argv[] = {"blsum", "-t", "bogus"};

  mute_stderr();
  retval = main(3, argv);
  unmute_stderr();

  assert_eq(2, retval);
}

test(main_shall_return_3_if_first_option_is_unknown) {
  int retval;
  char* argv[] = {"blsum", "bogus", "bogus"};

  mute_stderr();
  retval = main(3, argv);
  unmute_stderr();

  assert_eq(3, retval);
}

/*
 * Since all functions are stubbed, we have to search for the proxy function
 * verision of done() intead.
 */
#define done __tarsio_proxy_done
extern int __tarsio_proxy_done(story_t* story);

test(main_shall_call_backlog_read_with_correct_arguments_done) {
  char* argv[] = {"program_name_here", "-d", "foo.org"};
  const int make_sure_function_call_retval_is_returned = 12;
  m.backlog_read.retval = 12;
  mute_stdout();
  int retval = main(3, argv);
  unmute_stdout();
  assert_eq(make_sure_function_call_retval_is_returned, retval);
  assert_eq(0, strcmp(m.backlog_read.args.arg0, "foo.org"));
  assert_eq(done, m.backlog_read.args.arg1);
}

#undef done

/*
 * Since all functions are stubbed, we have to search for the proxy function
 * verision of todo_min() intead.
 */
#define todo_min __tarsio_proxy_todo_min
extern int __tarsio_proxy_todo_min(story_t* story);

test(main_shall_call_backlog_read_with_correct_arguments_todo_min) {
  char* argv[] = {"program_name_here", "-t", "-n", "foo.org"};
  const int make_sure_function_call_retval_is_returned = 12;
  m.backlog_read.retval = 12;
  mute_stdout();
  int retval = main(4, argv);
  unmute_stdout();
  assert_eq(make_sure_function_call_retval_is_returned, retval);
  assert_eq(0, strcmp(m.backlog_read.args.arg0, "foo.org"));
  assert_eq(todo_min, m.backlog_read.args.arg1);
}

#undef todo_min

/*
 * Since all functions are stubbed, we have to search for the proxy function
 * verision of todo_max() intead.
 */
#define todo_max __tarsio_proxy_todo_max
extern int __tarsio_proxy_todo_max(story_t* story);

test(main_shall_call_backlog_read_with_correct_arguments_todo_max) {
  char* argv[] = {"program_name_here", "-t", "-x", "foo.org"};
  const int make_sure_function_call_retval_is_returned = 12;
  m.backlog_read.retval = 12;
  mute_stdout();
  int retval = main(4, argv);
  unmute_stdout();
  assert_eq(make_sure_function_call_retval_is_returned, retval);
  assert_eq(0, strcmp(m.backlog_read.args.arg0, "foo.org"));
  assert_eq(todo_max, m.backlog_read.args.arg1);
}

#undef todo_max

