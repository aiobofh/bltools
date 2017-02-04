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
  assert_eq((char*)(1234 + strlen("yyyy-mm-dd ")),
            cutest_mock.has_date.args.arg0)
}

static int has_date_stub_no_start_but_end(const char* str) {
  if ((char*)1234 == str) { /* Test representation of a string pointer */
    return 0;
  }
  return 1;
}

static int has_date_stub_no_end_but_start(const char* str) {
  if ((char*)1234 == str) { /* Test representation of a string pointer */
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
 * has_commitment()
 */
test(has_commitment_shall_return_0_if_no_commitment_is_found) {
  assert_eq(0, has_commitment("yyyy-mm-dd yyyy-mm-dd")); /* Too short */
  assert_eq(0, has_commitment("yyyy-mm-dd yyyy-mm-dd ")); /* Too short */
  assert_eq(0, has_commitment("yyyy-mm-dd yyyy-mm-dd 13")); /* Too short */
  assert_eq(0, has_commitment("yyyy-mm-dd yyyy-mm-dd ID-directly"));
}

test(has_commitment_shall_return_1_if_commitment_is_found) {
  assert_eq(1, has_commitment("yyyy-mm-dd yyyy-mm-dd 13 "));
}

/*
 * has_id()
 */
test(has_id_shall_return_0_if_no_id_is_found) {
  assert_eq(0, has_id("yyyy-mm-dd yyyy-mm-dd nn")); /* Too short */
  assert_eq(0, has_id("yyyy-mm-dd yyyy-mm-dd nn ")); /* Too short */
}

test(has_id_shall_return_0_if_id_contains_other_than_alpha_numerics) {
  assert_eq(0, has_id("yyyy-mm-dd yyyy-mm-dd nn A b c d"));
  assert_eq(0, has_id("yyyy-mm-dd yyyy-mm-dd nn 123 123"));
}

test(has_id_shall_return_1_if_id_is_found) {
  assert_eq(1, has_id("yyyy-mm-dd yyyy-mm-dd nn Sprint-102_a"));
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
 * get_commitment()
 */
test(get_commitment_shall_return_the_commitment_as_an_integer) {
  assert_eq(13, get_commitment("yyyy-mm-dd yyyy-mm-dd 13 Sprint-tjosan"));
}

/*
 * get_id_length()
 */
test(get_id_length_shall_return_the_length_of_the_id) {
  assert_eq(17, get_id_length("yyyy-mm-dd yyyy-mm-dd nn Title-title-title"));
}

test(get_id_length_shall_return_the_length_of_the_id_trimmed_from_ws) {
  assert_eq(17, get_id_length("yyyy-mm-dd yyyy-mm-dd nn Title-title-title  "));
}

/*
 * get_id()
 */
test(get_id_shall_copy_the_id_from_a_sprint_to_dest) {
  char dest[32];
  get_id(dest, 17, "yyyy-mm-dd yyyy-mm-dd nn Title-title-title");
  assert_eq(0 == strcmp(dest, "Title-title-title"));
}

/*
 * is_sprint()
 */
test(is_sprint_should_rely_on_helpers_since_they_are_already_tested) {
  cutest_mock.has_dates.retval = 1;
  cutest_mock.has_commitment.retval = 1;
  cutest_mock.has_id.retval = 1;

  (void)is_sprint((char*)1234);

  /* has_dates() */
  assert_eq(1, cutest_mock.has_dates.call_count);
  assert_eq((char*)1234, cutest_mock.has_dates.args.arg0);

  /* has_commitment() */
  assert_eq(1, cutest_mock.has_commitment.call_count);
  assert_eq((char*)1234, cutest_mock.has_commitment.args.arg0);

  /* has_id() */
  assert_eq(1, cutest_mock.has_id.call_count);
  assert_eq((char*)1234, cutest_mock.has_id.args.arg0);
}

test(is_sprint_should_return_0_if_has_dates_fails) {
  cutest_mock.has_dates.retval = 0;
  cutest_mock.has_commitment.retval = 1;
  cutest_mock.has_id.retval = 1;

  assert_eq(0, is_sprint((char*)1234));
}

test(is_sprint_should_return_0_if_has_commitment_fails) {
  /* Note, this implies calling order is: has_orgmode_todo->has_estimate */
  cutest_mock.has_dates.retval = 1;
  cutest_mock.has_commitment.retval = 0;
  cutest_mock.has_id.retval = 1;

  assert_eq(0, is_sprint((char*)1234));
}

test(is_sprint_should_return_0_if_has_id_fails) {
  /* Note, this implies calling order is: has_orgmode_todo->has_estimate */
  cutest_mock.has_dates.retval = 1;
  cutest_mock.has_commitment.retval = 1;
  cutest_mock.has_id.retval = 0;

  assert_eq(0, is_sprint((char*)1234));
}

/* TODO: Switch to module_test() macro when supported in cutest. */
test(is_sprint_should_assess_sprints_correctly) {
  /* Integrate helpers by not mocking them */
  /* TODO: Remove these when module_test() macro is done in cutest. */
  cutest_mock.is_date.retval = 1;
  cutest_mock.has_date.func = has_date;
  cutest_mock.has_dates.func = has_dates;
  cutest_mock.has_id.func = has_id;
  cutest_mock.has_commitment.func = has_commitment;

  /* Top-level kick-the-tires test of is_story() */
  assert_eq(1, is_sprint("2017-01-01 2017-02-02 13 201701"));

  cutest_mock.is_date.retval = 0;

  assert_eq(0, is_sprint("However, this is not"));
}

/*
 * sprint_init()
 */
test(sprint_init_shall_call_has_dates_correctly) {
  sprint_init((sprint_t*)1234, (char*)5678);
  assert_eq(1, cutest_mock.has_dates.call_count);
  assert_eq((char*)5678, cutest_mock.has_dates.args.arg0);
}

test(sprint_init_shall_call_has_commitment_correctly) {
  cutest_mock.has_dates.retval = 1;
  sprint_init((sprint_t*)1234, (char*)5678);
  assert_eq(1, cutest_mock.has_commitment.call_count);
  assert_eq((char*)5678, cutest_mock.has_commitment.args.arg0);
}

test(sprint_init_shall_call_has_id_correctly) {
  cutest_mock.has_dates.retval = 1;
  cutest_mock.has_commitment.retval = 1;
  sprint_init((sprint_t*)1234, (char*)5678);
  assert_eq(1, cutest_mock.has_id.call_count);
  assert_eq((char*)5678, cutest_mock.has_id.args.arg0);
}

test(sprint_init_shall_call_get_id_length_correctly_if_id_and_dates) {
  cutest_mock.has_dates.retval = 4321;
  cutest_mock.has_commitment.retval = 1928;
  cutest_mock.has_id.retval = 8765;

  sprint_init((sprint_t*)1234, (char*)5678);

  assert_eq(1, cutest_mock.get_id_length.call_count);
  assert_eq((char*)5678, cutest_mock.get_id_length.args.arg0);
}

#define NORMAL_INIT_FIXTURE                     \
  cutest_mock.has_dates.retval = 4321;          \
  cutest_mock.has_id.retval = 8765;             \
  cutest_mock.has_commitment.retval = 1827;     \
  cutest_mock.get_id_length.retval = 1827;      \
                                                \
  sprint_t sprint;                              \
                                                \
  sprint_init(&sprint, (char*)5678)

test(sprint_init_shall_call_get_start_date_correctly) {
  NORMAL_INIT_FIXTURE;
  assert_eq(1, cutest_mock.get_start_date.call_count);
  assert_eq(&sprint.start, cutest_mock.get_start_date.args.arg0);
  assert_eq((char*)5678, cutest_mock.get_start_date.args.arg1);
}

 test(sprint_init_shall_call_get_end_date_correctly) {
  NORMAL_INIT_FIXTURE;
  assert_eq(1, cutest_mock.get_end_date.call_count);
  assert_eq(&sprint.end, cutest_mock.get_end_date.args.arg0);
  assert_eq((char*)5678, cutest_mock.get_end_date.args.arg1);
}

test(sprint_init_shall_malloc_correctly) {
  cutest_mock.malloc.retval = (void*)3645;
  NORMAL_INIT_FIXTURE;
  assert_eq(1, cutest_mock.malloc.call_count);
  assert_eq(1827 + 1, cutest_mock.malloc.args.arg0);
  assert_eq((char*)3645, sprint.id);
}

test(sprint_init_shall_call_get_commitment_correctly) {
  cutest_mock.malloc.retval = (void*)3645;
  NORMAL_INIT_FIXTURE;
  assert_eq(1, cutest_mock.get_commitment.call_count);
  assert_eq((char*)5678, cutest_mock.get_commitment.args.arg0);
}

test(sprint_init_shall_call_get_id_correctly) {
  cutest_mock.malloc.retval = (void*)3645;
  NORMAL_INIT_FIXTURE;
  assert_eq(1, cutest_mock.get_id.call_count);
  assert_eq((char*)3645, cutest_mock.get_id.args.arg0);
  assert_eq(1827, cutest_mock.get_id.args.arg1);
  assert_eq((char*)5678, cutest_mock.get_id.args.arg2);
}

static void get_id_stub(char* dest, size_t len, const char* str) {
  strcpy(dest, "Sprint-123_b");
  len = len;
  str = str;
}

test(sprint_init_shall_call_get_id_with_correct_arguments) {
  char destination[13]; /* strlen("Sprint-123_b") + 1 */
  cutest_mock.malloc.retval = (void*)destination;
  cutest_mock.get_id.func = get_id_stub;
  NORMAL_INIT_FIXTURE;

  assert_eq(0 == strcmp(sprint.id, "Sprint-123_b"));
}

test(sprint_cleanup_shall_free_the_memory_allocated_for_sprint_id_string) {
  sprint_t sprint;
  sprint_cleanup(&sprint);
  assert_eq(1, cutest_mock.free.call_count);
  assert_eq(sprint.id, cutest_mock.free.args.arg0);
}
