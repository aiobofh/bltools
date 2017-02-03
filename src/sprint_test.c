#include "cutest.h"

/*
 * has_date()
 */
test(has_date_shall_call_is_date_with_correct_arguments_return_0_on_fail) {
  /* By default is_date mock returns 0 = fail */
  assert_eq(0, has_date((char*)1234));
  assert_eq(1, cutest_mock.is_date.call_count);
  assert_eq((char*)1234, cutest_mock.is_date.args.arg0);
}

test(has_date_shall_return_0_date_does_not_end_with_space) {
  cutest_mock.is_date.retval = 1;
  assert_eq(0, has_date("yyyy-mm-ddd"));
}

test(has_date_shall_return_1_date_does_end_with_space) {
  cutest_mock.is_date.retval = 1;
  assert_eq(1, has_date("yyyy-mm-dd "));
}

/*
 * has_dates()
 */
test(has_dates_shall_call_has_date_for_start_date_with_correct_argument) {
  cutest_mock.has_date.retval = 0;
  (void)has_dates((char*)1234);
  assert_eq((char*)1234, cutest_mock.has_date.args.arg0)
}

test(has_dates_shall_call_has_date_for_end_date_with_correct_argument) {
  cutest_mock.has_date.retval = 1;
  (void)has_dates((char*)1234);
  assert_eq((char*)(1234 + strlen("yyyy-mm-dd ") - 1),
            cutest_mock.has_date.args.arg0)
}

static int has_date_stub_no_start_but_end(const char* str) {
  if ((char*)1234 == str) {
    return 0;
  }
  return 1;
}

static int has_date_stub_no_end_but_start(const char* str) {
  if ((char*)1234 == str) {
    return 1;
  }
  return 0;
}

test(has_dates_shall_return_0_if_the_sprint_has_no_start_but_end_date) {
  cutest_mock.has_date.func = has_date_stub_no_start_but_end;
  assert_eq(0, has_dates((char*)1234));
}

test(has_dates_shall_return_0_if_the_sprint_has_no_end_but_start_date) {
  cutest_mock.has_date.func = has_date_stub_no_end_but_start;
  assert_eq(0, has_dates((char*)1234));
}

test(has_dates_shall_return_1_if_the_sprint_has_both_start_and_end_dates) {
  cutest_mock.has_date.retval = 1;
  assert_eq(1, has_dates((char*)1234));
}

/*
 * has_title()
 */
test(has_title_shall_return_0_if_no_title_is_found) {
  assert_eq(0, has_title("yyyy-mm-dd yyyy-mm-dd")); /* Too short */
  assert_eq(0, has_title("yyyy-mm-dd yyyy-mm-dd ")); /* Too short */
}

test(has_title_shall_return_1_if_title_is_found) {
  assert_eq(1, has_title("yyyy-mm-dd yyyy-mm-dd Title"));
}

/*
 * get_date()
 */
test(get_date_shall_convert_date_correclty) {
  date_t d;
  get_date(&d, "1234-56-78");
  assert_eq(1234, d.year);
  assert_eq(56, d.month);
  assert_eq(78, d.day);
}

/*
 * get_start_date()
 */
test(get_start_shall_call_get_date_correclty) {
  get_start_date((date_t*)1234, (char*)5678);
  assert_eq(1, cutest_mock.get_date.call_count);
  assert_eq((date_t*)1234, cutest_mock.get_date.args.arg0);
  assert_eq((char*)5678, cutest_mock.get_date.args.arg1);
}

/*
 * get_end_date()
 */
test(get_end_date_shall_convert_date_correclty) {
  get_end_date((date_t*)1234, (char*)5678);
  assert_eq(1, cutest_mock.get_date.call_count);
  assert_eq((date_t*)1234, cutest_mock.get_date.args.arg0);
  assert_eq((char*)5678 + strlen("yyyy-mm-dd "),
            cutest_mock.get_date.args.arg1);
}

/*
 * get_title_length()
 */
test(get_title_length_shall_return_the_length_of_the_title) {
  assert_eq(5, get_title_length("yyyy-mm-dd yyy-mm-dd Title"));
  assert_eq(11, get_title_length("yyyy-mm-dd yyy-mm-dd Title title"));
  assert_eq(17, get_title_length("yyyy-mm-dd yyy-mm-dd Title title title"));
}

/*
 * get_title()
 */
test(get_title_shall_copy_the_title_from_a_sprint_to_dest) {
  char dest[32];
  get_title(dest, 5, "yyyy-mm-dd yyy-mm-dd Title");
  assert_eq(0 == strcmp(dest, "Title"));

  get_title(dest, 11, "yyyy-mm-dd yyy-mm-dd Title title");
  assert_eq(0 == strcmp(dest, "Title title"));

  get_title(dest, 17, "yyyy-mm-dd yyy-mm-dd Title title title");
  assert_eq(0 == strcmp(dest, "Title title title"));
}
