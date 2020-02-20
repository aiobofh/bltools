#include <unistd.h>

#include <tarsio.h>
#include "sprint.h"
#include "backlog.h"
#include "blsprints_data.h"

#define m tarsio_mock

static int muted_stderr_fd;
static fpos_t stderr_pos;

static int muted_stdout_fd;
static fpos_t stdout_pos;

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

/*
 * get_row()
 */
test(get_row_shall_read_a_file_row_correctly) {
  m.fgets.retval = (char*)4321;

  (void)get_row((char*)1234, (FILE*)5678, (char*)8765, 0);

  assert_eq((char*)1234, m.fgets.args.arg0);
  assert_eq(512, m.fgets.args.arg1);
  assert_eq((FILE*)5678, m.fgets.args.arg2);
}

test(get_row_shall_return_null_on_end_of_file) {
  m.feof.retval = 4321;

  assert_eq(NULL, get_row((char*)1234, (FILE*)5678, (char*)8765, 0));
}

test(get_row_shall_return_null_on_file_read_eror) {
  mute_stderr();
  assert_eq(NULL, get_row((char*)1234, (FILE*)5678, "filename", 0));
  unmute_stderr();
}

test(get_row_shall_return_fgets_retval_on_success) {
  m.fgets.retval = (char*)4321;
  assert_eq((char*)4321, get_row((char*)1234, (FILE*)5678, (char*)8765, 0));
}

/*
 * rtrim()
 */
test(rtrim_shall_inplace_remove_whitespaces_at_end_of_the_string) {
  char str[32];

  m.strlen.func = strlen;
  m.__ctype_b_loc.func = __ctype_b_loc;

  strcpy(str, "This is a string \t\r\n\n\t ");

  rtrim(str);

  assert_eq(0, strcmp(str, "This is a string"));
}

test(rtrim_shall_not_break_empty_string) {
  char str[32];

  strcpy(str, "");

  rtrim(str);

  assert_eq(0, strcmp(str, ""));
}

/*
 * print_sprint()
 */
test(print_sprint_shall_call_sprint_init_correctly) {
  print_sprint((char*)1234);

  assert_eq((char*)1234, m.sprint_init.args.arg1);
}

test(print_sprint_shall_call_puts_correctly) {
  print_sprint((char*)1234);
  assert_eq(m.sprint_init.args.arg0->id,
            m.fputs.args.arg0);
  assert_eq('\n', m._IO_putc.args.arg0);
}

test(print_sprint_shall_call_sprint_cleanup_correctly) {
  mute_stdout();
  print_sprint((char*)1234);
  unmute_stdout();

  assert_eq(m.sprint_init.args.arg0,
            m.sprint_cleanup.args.arg0);
}

/*
 * parse_and_print()
 */
test(parse_and_print_shall_call_get_row_correctly) {
  (void)parse_and_print((FILE*)1234, (char*)5678, 9);

  assert_eq(1, m.get_row.call_count);
  assert_eq((FILE*)1234, m.get_row.args.arg1);
  assert_eq((char*)5678, m.get_row.args.arg2);
  assert_eq(9, m.get_row.args.arg3);
}

test(parse_and_print_shall_return_1_if_get_row_fails) {
  assert_eq(1, parse_and_print((FILE*)1234, (char*)5678, 9));
}

test(parse_and_print_shall_call_rtrim_correctly) {
  m.get_row.retval = (char*)4321;
  m.is_sprint.retval = 1; /* Avoid printout */

  (void)parse_and_print((FILE*)1234, (char*)5678, 9);

  assert_eq(1, m.rtrim.call_count);
  assert_eq(m.get_row.args.arg0, m.rtrim.args.arg0);
}

test(parse_and_print_shall_return_2_if_is_sprint_return_false) {
  m.get_row.retval = (char*)4321;

  mute_stderr();
  assert_eq(2, parse_and_print((FILE*)1234, "filename", 9));
  unmute_stderr();
}

test(parse_and_print_shall_call_print_sprint_correctly) {
  m.get_row.retval = (char*)4321;
  m.is_sprint.retval = 8765;

  (void)parse_and_print((FILE*)1234, (char*)5678, 9);

  assert_eq(1, m.print_sprint.call_count);
  assert_eq(m.get_row.args.arg0,
            m.print_sprint.args.arg0);
}

test(parse_and_print_shall_return_0_on_success) {
  m.get_row.retval = (char*)4321;
  m.is_sprint.retval = 8765;

  assert_eq(0, parse_and_print((FILE*)1234, (char*)5678, 9));
}

/*
 * sprint_simple_read()
 */
test(sprints_simple_read_shall_call_fopen_correctly) {
  const char* sprintfile = "sprintfile";

  mute_stderr();
  (void)sprints_simple_read(sprintfile);
  unmute_stderr();

  assert_eq(1, m.fopen.call_count);
  assert_eq(sprintfile, m.fopen.args.arg0);
}

test(sprints_simple_read_shall_return_1_if_fopen_fails_to_open_file) {
  mute_stderr();
  assert_eq(1, sprints_simple_read("sprintfile"));
  unmute_stderr();
}

static int feof_4_times_stub_calls;
static int feof_4_times_stub(FILE* stream) {
  stream = stream;
  if (feof_4_times_stub_calls > 0) {
    feof_4_times_stub_calls--;
    return 0;
  }
  return 1;
}

test(sprints_simple_read_shall_read_the_file_until_feof_returns_nonzero) {
  feof_4_times_stub_calls = 4;

  m.fopen.retval = (FILE*)4321;
  m.feof.func = feof_4_times_stub;

  (void)sprints_simple_read((char*)1234);

  assert_eq(0, feof_4_times_stub_calls);

  feof_4_times_stub_calls = 4;
}

test(sprints_simple_read_shall_call_parse_and_print_correctly_for_every_file_row) {
  feof_4_times_stub_calls = 4;
  m.fopen.retval = (FILE*)4321;
  m.feof.func = feof_4_times_stub;

  (void)sprints_simple_read((char*)1234);

  assert_eq(4, m.parse_and_print.call_count);
  assert_eq((FILE*)4321, m.parse_and_print.args.arg0);

  feof_4_times_stub_calls = 4;
}

test(sprints_simple_read_shall_close_file_and_return_2_if_parse_and_print_fails) {
  feof_4_times_stub_calls = 4;
  m.fopen.retval = (FILE*)4321;
  m.parse_and_print.retval = 8765;

  assert_eq(2, sprints_simple_read((char*)1234));
  /* Abort on first loop */
  assert_eq(1, m.parse_and_print.call_count);
  assert_eq(1, m.fclose.call_count);
  assert_eq((FILE*)4321, m.fclose.args.arg0);
}

test(sprints_simple_read_shall_close_the_file_and_return_0_at_end_of_file) {
  feof_4_times_stub_calls = 4;
  m.fopen.retval = (FILE*)4321;
  m.feof.retval = 1;

  assert_eq(0, sprints_simple_read((char*)1234));
  assert_eq(1, m.fclose.call_count);
  assert_eq((FILE*)4321, m.fclose.args.arg0);
}

/*
 * main()
 */
test(main_shall_return_EXIT_FAILURE_on_too_few_arguments) {
  mute_stderr();
  assert_eq(EXIT_FAILURE, __tarsio_replace_main(1, NULL));
  unmute_stderr();
}

test(main_shall_return_EXIT_FAILURE_on_too_many_arguments) {
  mute_stderr();
  assert_eq(EXIT_FAILURE, __tarsio_replace_main(3, NULL));
  unmute_stderr();
}

test(main_shall_call_sprints_simple_read_correctly) {
  char* argv[] = {(char*)1234, (char*)5678};

  (void)__tarsio_replace_main(2, argv);

  assert_eq(1, m.sprints_simple_read.call_count);
  assert_eq((char*)5678, m.sprints_simple_read.args.arg0);
}

test(main_shall_return_EXIT_FAILURE_when_sprits_read_fails) {
  char* argv[] = {(char*)1234, (char*)5678};

  m.sprints_simple_read.retval = 4321;

  assert_eq(EXIT_FAILURE, __tarsio_replace_main(2, argv));
}

test(main_shall_return_EXIT_SUCCESS_when_all_went_good) {
  char* argv[] = {(char*)1234, (char*)5678};

  assert_eq(EXIT_SUCCESS, __tarsio_replace_main(2, argv));
}
