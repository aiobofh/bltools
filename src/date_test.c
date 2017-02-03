#include "cutest.h"

test(is_date_shall_return_0_if_string_does_not_start_with_year) {
  assert_eq(0, is_date("jfdsalkfdasklfjsdalkfj"));
}

test(is_date_shall_return_0_if_string_missing_first_separator) {
  assert_eq(0, is_date("123412-12"));
}

test(is_date_shall_return_0_if_string_does_not_have_month) {
  assert_eq(0, is_date("1234-ab-12"));
}

test(is_date_shall_return_0_if_string_missing_second_separator) {
  assert_eq(0, is_date("1234-1212"));
}

test(is_date_shall_return_0_if_string_does_not_have_day) {
  assert_eq(0, is_date("1234-12-ab"));
}

test(is_date_shall_return_1_if_string_is_a_valid_date) {
  assert_eq(1, is_date("1234-12-12"));
}
