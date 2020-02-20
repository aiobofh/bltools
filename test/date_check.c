#include <time.h>

#include <tarsio.h>
#include "date.h"
#include "date_data.h"

#define m tarsio_mock

/*
 * is_date()
 */
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

/*
 * date2long()
 */
test(date2long_shall_convert_a_date_into_a_long_integer) {
  date_t date;
  date.year = 2017; date.month = 2; date.day = 4; /* Today :) */
  assert_eq(20170204, date2long(&date));
}

/*
 * long2date()
 */
test(long2date_shall_convert_a_long_integer_to_a_date) {
  date_t dest;
  long2date(&dest, 20170204);
  assert_eq(2017, dest.year);
  assert_eq(2, dest.month);
  assert_eq(4, dest.day);
}

/*
 * dateadd()
 */
test(dateadd_shall_add_a_day_to_the_input_date_at_month_end) {
  date_t date;
  date.year = 2017; date.month = 2; date.day = 28;
  m.bzero.func = bzero;
  m.mktime.func = mktime;
  m.gmtime.func = gmtime;
  dateadd(&date);
  assert_eq(2017, date.year);
  assert_eq(3, date.month);
  assert_eq(1, date.day);
}

test(dateadd_shall_add_a_day_to_the_input_date_at_year_end) {
  date_t date;
  m.bzero.func = bzero;
  m.mktime.func = mktime;
  m.gmtime.func = gmtime;
  date.year = 2017; date.month = 12; date.day = 31;
  dateadd(&date);
  assert_eq(2018, date.year);
  assert_eq(1, date.month);
  assert_eq(1, date.day);
}

/*
 * date2shortname()
 */
#define DATE2SHORTNAME_FIXTURE(WDAY) \
  char shortname[3] = {0, 0, 0};     \
  date_t date;                       \
  struct tm tm;                      \
  m.gmtime.retval = &tm;   \
  tm.tm_wday = WDAY

test(date2shortname_shall_set_destination_to_Mo_if_date_is_a_monday) {
  DATE2SHORTNAME_FIXTURE(1);
  date2shortname(shortname, &date);
  assert_eq(0, strcmp("Mo", shortname));
}

test(date2shortname_shall_set_destination_to_Tu_if_date_is_a_tuesday) {
  DATE2SHORTNAME_FIXTURE(2);
  date2shortname(shortname, &date);
  assert_eq(0, strcmp("Tu", shortname));
}

test(date2shortname_shall_set_destination_to_We_if_date_is_a_wednesday) {
  DATE2SHORTNAME_FIXTURE(3);
  date2shortname(shortname, &date);
  assert_eq(0, strcmp("We", shortname));
}

test(date2shortname_shall_set_destination_to_Th_if_date_is_a_thursday) {
  DATE2SHORTNAME_FIXTURE(4);
  date2shortname(shortname, &date);
  assert_eq(0, strcmp("Th", shortname));
}

test(date2shortname_shall_set_destination_to_Fr_if_date_is_a_friday) {
  DATE2SHORTNAME_FIXTURE(5);
  date2shortname(shortname, &date);
  assert_eq(0, strcmp("Fr", shortname));
}

test(date2shortname_shall_set_destination_to_Sa_if_date_is_a_saturday) {
  DATE2SHORTNAME_FIXTURE(6);
  date2shortname(shortname, &date);
  assert_eq(0, strcmp("Sa", shortname));
}

test(date2shortname_shall_set_destination_to_Su_if_date_is_a_sunday) {
  DATE2SHORTNAME_FIXTURE(0);
  date2shortname(shortname, &date);
  assert_eq(0, strcmp("Su", shortname));
}
