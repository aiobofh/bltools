#include <time.h>

#include <tarsio.h>
#include "story.h"
#include "story_data.h"

#define m tarsio_mock

/*
 * has_orgmode_todo()
 */
test(has_orgmode_todo_shall_return_0_if_no_row_is_provided) {
  assert_eq(0, has_orgmode_todo(NULL));
}

test(has_orgmode_todo_shall_return_0_if_row_does_not_start_with_star) {
  assert_eq(0, has_orgmode_todo(" * TODO "));
  assert_eq(0, has_orgmode_todo("# DONE "));
}

test(has_orgmode_todo_shall_return_0_if_length_is_too_short) {
  assert_eq(0, has_orgmode_todo("*"));
}

test(has_orgmode_todo_shall_return_0_if_row_dows_not_contain_todo_or_done) {
  m.strncmp.retval = -1;
  assert_eq(0, has_orgmode_todo("* Slogan"));
}

test(has_orgmode_todo_shall_return_0_if_there_are_too_many_spaces) {
  assert_eq(0, has_orgmode_todo("* TODO  "));
  assert_eq(0, has_orgmode_todo("*  TODO "));
}

test(has_orgmode_todo_shall_return_1_if_row_contains_todo_or_done) {
  assert_eq(1, has_orgmode_todo("* TODO "));
  assert_eq(1, has_orgmode_todo("* DONE "));
}

/*
 * has_estimate()
 */
test(has_estimate_shall_return_0_if_no_row_is_provided) {
  assert_eq(0, has_estimate(0, NULL));
}

test(has_estimate_shall_return_0_if_ogmode_todo_argument_is_false) {
  assert_eq(0, has_estimate(0, NULL));
  assert_eq(0, has_estimate(0, "* TODO 00 "));
}

test(has_estimate_shall_return_0_if_the_row_has_no_estimate) {
  m.strlen.func = strlen;
  assert_eq(0, has_estimate(1, "* TODO Slogan "));
}

test(has_estimate_shall_return_0_if_there_is_no_space_or_dash_after) {
  m.strlen.func = strlen;
  assert_eq(0, has_estimate(1, "* TODO 03Slogan "));
}

test(has_estimate_shall_return_0_if_the_row_is_too_short) {
  m.strlen.func = strlen;
  assert_eq(0, has_estimate(1, "* TODO"));
}

test(has_estimate_shall_return_0_if_there_are_too_many_spaces) {
  m.strlen.func = strlen;
  assert_eq(0, has_estimate(1, "* TODO 01  Slogan"));
}

test(has_estimate_shall_return_1_if_the_row_has_an_estimate) {
  m.strlen.func = strlen;
  assert_eq(1, has_estimate(1, "* TODO 00 "));
  assert_eq(1, has_estimate(1, "* TODO 01 "));
  assert_eq(1, has_estimate(1, "* TODO 01-02"));
}

/*
 * has_estimate_range()
 */
test(has_estimate_range_shall_return_0_if_has_estimate_bool_is_false) {
  m.strlen.func = strlen;
  assert_eq(0, has_estimate_range(0, NULL));
}

test(has_estimate_range_shall_return_0_no_range_is_defined) {
  m.strlen.func = strlen;
  assert_eq(0, has_estimate_range(1, "* TODO 00 Slogan"));
}

test(has_estimate_range_shall_return_0_range_is_broken) {
  m.strlen.func = strlen;
  assert_eq(0, has_estimate_range(1, "* TODO 0001"));
}

test(has_estimate_range_shall_return_0_if_no_slogan_after_range) {
  m.strlen.func = strlen;
  assert_eq(0, has_estimate_range(1, "* TODO 00-01"));
}

test(has_estimate_range_shall_return_0_if_too_many_spaces) {
  m.strlen.func = strlen;
  assert_eq(0, has_estimate_range(1, "* TODO 00-01  "));
}

test(has_estimate_range_shall_return_1_if_range_is_defined) {
  m.strlen.func = strlen;
  assert_eq(1, has_estimate_range(1, "* TODO 00-01 Slogan"));
}

/*
 * is_date_formatted()
 */
test(is_date_formatted_shall_call_is_date_with_correct_argument) {
  m.strlen.func = strlen;
  assert_eq(0, is_date_formatted((char*)1234));
  assert_eq(1, m.is_date.call_count);
  assert_eq((char*)1234, m.is_date.args.arg0);
}

test(is_date_formatted_shall_return_0_if_date_suddanly_contains_lt) {
  m.strlen.func = strlen;
  m.is_date.retval = 1;
  assert_eq(0, is_date_formatted("1234-12-12<"));
}

test(is_date_formatted_shall_return_0_if_does_not_end_with_gt_gt) {
  m.strlen.func = strlen;
  m.is_date.retval = 1;
  assert_eq(0, is_date_formatted("1234-12-12"));
}

test(is_date_formatted_shall_return_1_if_string_start_with_date_and_gt) {
  m.strlen.func = strlen;
  m.is_date.retval = 1;
  assert_eq(1, is_date_formatted("1234-12-12>"));
}

/*
 * has_date()
 */
test(has_date_shall_return_0_if_needle_is_not_found_in_haystack) {
  m.strlen.func = strlen;
  m.strstr.func = strstr;
  assert_eq(0, has_date("haystack", "needle"));
}

test(has_date_call_is_date_formatted_correctly_if_needle_is_found) {
  m.strlen.func = strlen;
  m.strstr.func = strstr;
  const char* str = "haystack needle haystack";
  assert_eq(0, has_date(str, "needle"));
  assert_eq(1, m.is_date_formatted.call_count);
  assert_eq((char*)(str + 9 + strlen("needle")),
            m.is_date_formatted.args.arg0);
}

/*
 * has_scheduled();
 */
test(has_scheduled_shall_call_has_date_correclty_and_forward_retval) {
  const char* str = "A silly string";

  m.has_date.retval = 1234;

  assert_eq(1234, has_scheduled(str));
  assert_eq(str, m.has_date.args.arg0);
  assert_eq(0, strcmp("SCHEDULED: <", m.has_date.args.arg1));
}

/*
 * has_deadline();
 */
test(has_deadline_shall_call_has_date_correclty_and_forward_retval) {
  const char* str = "A silly string";

  m.has_date.retval = 1234;

  assert_eq(1234, has_deadline(str));
  assert_eq(str, m.has_date.args.arg0);
  assert_eq(0, strcmp("DEADLINE: <", m.has_date.args.arg1));
}

/*
 * has_interval()
 */
test(has_interval_shall_return_0_if_no_row_is_provided) {
  assert_eq(0, has_interval(1, NULL));
}

test(has_interval_shall_return_0_if_no_story_is_provided) {
  assert_eq(0, has_interval(0, (char*)5432));
}

test(has_interval_shall_call_has_scheduled_correctly) {
  assert_eq(0, has_interval(1, (char*)5432));
  assert_eq(1, m.has_scheduled.call_count);
  assert_eq((char*)5432, m.has_scheduled.args.arg0);
}

test(has_interval_shall_call_has_deadline_correctly) {
  assert_eq(0, has_interval(1, (char*)5432));
  assert_eq(1, m.has_deadline.call_count);
  assert_eq((char*)5432, m.has_deadline.args.arg0);
}

test(has_interval_shall_return_0_if_no_scheduled_but_deadline) {
  m.has_scheduled.retval = 0;
  m.has_deadline.retval = 1;
  assert_eq(0, has_interval(1, (char*)5432));
}

test(has_interval_shall_return_0_if_no_deadline_but_scheduled) {
  m.has_scheduled.retval = 1;
  m.has_deadline.retval = 0;
  assert_eq(0, has_interval(1, (char*)5432));
}

/*
 * is_story()
 */
test(is_story_should_rely_on_helpers_since_they_are_already_tested) {
  const char *str = "Sould be passed to helpers";

  m.has_orgmode_todo.retval = 1;
  m.has_estimate.retval = 1;

  (void)is_story(str);

  /* has_orgmode_todo() */
  assert_eq(1, m.has_orgmode_todo.call_count);
  assert_eq(0, strcmp(m.has_orgmode_todo.args.arg0, str));

  /* has_estimate() */
  assert_eq(1, m.has_estimate.call_count);
  assert_eq(1, m.has_estimate.args.arg0);
  assert_eq(0, strcmp(m.has_estimate.args.arg1, str));
}

test(is_story_should_return_0_if_has_orgmode_todo_fails) {
  const char *str = "Sould be passed to helpers";

  m.has_orgmode_todo.retval = 0;

  assert_eq(0, is_story(str));
}

test(is_story_should_return_0_if_has_orgmode_todo_ok_but_has_estimate_fails) {
  const char *str = "Sould be passed to helpers";

  /* Note, this implies calling order is: has_orgmode_todo->has_estimate */
  m.has_orgmode_todo.retval = 1;
  m.has_estimate.retval = 0;

  assert_eq(0, is_story(str));
}

/* TODO: Switch to module_test() macro when supported in cutest. */
test(is_story_should_assess_stories_correctly) {
  /* Integrate helpers by not mocking them */
  /* TODO: Remove these when module_test() macro is done in cutest. */
  m.strlen.func = strlen;
  m.strstr.func = strstr;
  m.has_orgmode_todo.func = has_orgmode_todo;
  m.has_estimate.func = has_estimate;

  /* Top-level kick-the-tires test of is_story() */
  assert_eq(1, is_story("* TODO 01 This is a story"));
  assert_eq(1, is_story("* TODO 01-02 This is a story"));
  assert_eq(1, is_story("* DONE 01 This is a story"));
  assert_eq(1, is_story("* DONE 01-02 This is a story"));
  assert_eq(1, is_story("* TODO 01 This is a story :tag:"));
  assert_eq(1, is_story("* TODO 01 This is a story       :tag:"));
  assert_eq(1, is_story("* TODO 01-02 This is a story :tag:"));
  assert_eq(1, is_story("* TODO 01-02 This is a story       :tag:"));
  assert_eq(1, is_story("* TODO 01-02 This is a story :tag1:tag2:"));
  assert_eq(1, is_story("* TODO 01-02 This is a story       :tag1:tag2:"));
  assert_eq(0, is_story("However, this is not"));
  assert_eq(0, is_story("* However, this is not"));
  assert_eq(0, is_story("TODO However, this is not"));
  assert_eq(0, is_story("DONE 01 However, this is not"));
  assert_eq(0, is_story("*  TODO 01 Nor is this"));
  assert_eq(0, is_story("* TODO  01 Nor is this"));
  assert_eq(0, is_story("* TODO 01  Nor is this"));
}

/*
 * get_status()
 */
test(get_status_should_return_STATUS_TODO_if_the_row_is_a_TODO_row) {
  m.strncmp.func = strncmp;
  assert_eq(STATUS_TODO, get_status(1, "* TODO Slogan"));
}

test(get_status_should_return_STATUS_DONE_if_the_row_is_a_DONE_row) {
  m.strncmp.func = strncmp;
  assert_eq(STATUS_DONE, get_status(1, "* DONE Slogan"));
}

/*
 * get_estimate()
 */
test(get_estimate_should_return_2_if_estimate_is_02) {
  assert_eq(2, get_estimate(1, "* DONE 02 Slogan"));
}

test(get_estimate_should_return_22_if_estimate_is_22) {
  assert_eq(22, get_estimate(1, "* DONE 22 Slogan"));
}

/*
 * sum_estimates()
 */
test(sum_min_estimates_shall_add_min_estimate_to_sum) {
  int sum=4;
  story_t story;

  story.estimate_type = ESTIMATE_POINTS;
  story.estimate.points = 1;
  sum_min_estimates(&sum, &story);
  assert_eq(5, sum);
}

test(sum_min_estimates_shall_add_min_estimate_from_range_to_sum) {
  int sum=4;
  story_t story;

  story.estimate_type = ESTIMATE_RANGE;
  story.estimate.range.min_points = 6;
  sum_min_estimates(&sum, &story);
  assert_eq(10, sum);
}

test(sum_max_estimates_shall_add_max_estimate_to_sum) {
  int sum=20;
  story_t story;

  story.estimate_type = ESTIMATE_POINTS;
  story.estimate.points = 30;
  sum_max_estimates(&sum, &story);
  assert_eq(50, sum);
}

test(sum_max_estimates_range_shall_add_max_estimate_from_range_to_sum) {
  int sum=40;
  story_t story;

  story.estimate_type = ESTIMATE_RANGE;
  story.estimate.range.max_points = 30;
  sum_max_estimates(&sum, &story);
  assert_eq(70, sum);
}

/*
 * get_max_estimate()
 */
test(get_max_estimate_should_return_2_if_max_estimate_is_02) {
  assert_eq(2, get_max_estimate(1, "* DONE 00-02 Slogan"));
}

test(get_max_estimate_should_return_22_if_max_estimate_is_22) {
  assert_eq(22, get_max_estimate(1, "* DONE 99-22 Slogan"));
}

/*
 * has_slogan()
 */
test(has_slogan_shall_return_0_if_no_orgmode_todo) {
  assert_eq(0, has_slogan(0, 1, 0, "* TODO 01 Slogan       :tag:"));
}

test(has_slogan_shall_return_0_if_no_estimate) {
  assert_eq(0, has_slogan(1, 0, 0, "* TODO 01 Slogan       :tag:"));
}

test(has_slogan_shall_return_1_if_a_story_has_a_slogan_with_estimate) {
  assert_eq(1, has_slogan(1, 1, 0, "* TODO 01 Slogan       :tag:"));
}

test(has_slogan_shall_return_0_if_only_spaces_or_no_slogan) {
  assert_eq(0, has_slogan(1, 1, 1, "* TODO 01        :tag:"));
  assert_eq(0, has_slogan(1, 1, 1, "* TODO 01 :tag:"));
}

test(has_slogan_shall_return_1_if_a_story_has_a_slogan_with_estimate_range) {
  assert_eq(1, has_slogan(1, 1, 1, "* TODO 01-03 Slogan       :tag:"));
}

/*
 * get_slogan_length()
 */
test(get_slogan_length_shall_return_0_if_no_slogan) {
  assert_eq(0, get_slogan_length(0, 0, "* TODO 01 Slogan    :tag:"));
}

test(get_slogan_length_shall_return_the_length_of_the_slogan) {
  m.strlen.func = strlen;

  assert_eq(6, get_slogan_length(1, 0, "* TODO 01 Slogan    :tag:"));
  assert_eq(6, get_slogan_length(1, 1, "* TODO 01-03 Slogan     :tag:"));
  assert_eq(9, get_slogan_length(1, 0, "* TODO 01 Slogannnn"));
  assert_eq(9, get_slogan_length(1, 1, "* TODO 01-03 Slogannnn"));
}

/*
 * get_slogan()
 */
test(get_slogan_shall_copy_the_slogan_from_a_story_to_dest) {
  char dest[7];
  m.memcpy.func = memcpy;
  get_slogan(dest, 0, 6, "* TODO 01 Slogan     :tag:");
  assert_eq(0, strcmp(dest, "Slogan"));

  get_slogan(dest, 1, 6, "* TODO 01-12 SloGan     :tag:");
  assert_eq(0, strcmp(dest, "SloGan"));

  get_slogan(dest, 0, 6, "* TODO 01 SloGAn");
  assert_eq(0, strcmp(dest, "SloGAn"));
}

/*
 * get_year()
 */
test(get_year_shall_return_1234_if_needle_is_found_in_haystack) {
  m.strstr.func = strstr;
  m.strlen.func = strlen;
  assert_eq(1234, get_year("foo: <1234", "foo: <"));
}

/*
 * get_month()
 */
test(get_month_shall_return_56_if_needle_is_found_in_haystack) {
  m.strstr.func = strstr;
  m.strlen.func = strlen;
  assert_eq(56, get_month("foo: <1234-56", "foo: <"));
}

/*
 * get_day()
 */
test(get_month_shall_return_78_if_needle_is_found_in_haystack) {
  m.strstr.func = strstr;
  m.strlen.func = strlen;
  assert_eq(78, get_day("foo: <1234-56-78", "foo: <"));
}

/*
 * get_scheduled_year()
 */
test(get_scheduled_year_shall_call_get_year_correctly_and_forward_retval) {
  m.get_year.retval = 1234;
  assert_eq(1234, get_scheduled_year("hobbla hobbla"));
  assert_eq(1, m.get_year.call_count);
}

/*
 * get_scheduled_month()
 */
test(get_scheduled_month_shall_call_get_month_correctly_and_forward_retval) {
  m.get_month.retval = 56;
  assert_eq(56, get_scheduled_month("hobbla hobbla"));
  assert_eq(1, m.get_month.call_count);
}

/*
 * get_scheduled_day()
 */
test(get_scheduled_day_shall_call_get_day_correctly_and_forward_retval) {
  m.get_day.retval = 78;
  assert_eq(78, get_scheduled_day("hobbla hobbla"));
  assert_eq(1, m.get_day.call_count);
}

/*
 * get_deadline_year()
 */
test(get_deadline_year_shall_call_get_year_correctly_and_forward_retval) {
  m.get_year.retval = 1234;
  assert_eq(1234, get_deadline_year("hobbla hobbla"));
  assert_eq(1, m.get_year.call_count);
}

/*
 * get_deadline_month()
 */
test(get_deadline_month_shall_call_get_month_correctly_and_forward_retval) {
  m.get_month.retval = 56;
  assert_eq(56, get_deadline_month("hobbla hobbla"));
  assert_eq(1, m.get_month.call_count);
}

/*
 * get_deadline_day()
 */
test(get_deadline_day_shall_call_get_day_correctly_and_forward_retval) {
  m.get_day.retval = 78;
  assert_eq(78, get_deadline_day("hobbla hobbla"));
  assert_eq(1, m.get_day.call_count);
}

/*
 * get_interval()
 */
#define GET_INTERVAL_FIXTURE                                            \
  date_t start;                                                         \
  date_t end;                                                           \
  const char* str = "SCHEDULED: <1234-56-78> DEADLINE: <8765-43-21>";   \
                                                                        \
  get_interval(&start, &end, str)

test(get_interval_shall_call_get_scheduled_year_correctly) {
  GET_INTERVAL_FIXTURE;
  assert_eq(1, m.get_scheduled_year.call_count);
  assert_eq(str, m.get_scheduled_year.args.arg0);
}

test(get_interval_shall_call_get_scheduled_month_correctly) {
  GET_INTERVAL_FIXTURE;
  assert_eq(1, m.get_scheduled_month.call_count);
  assert_eq(str, m.get_scheduled_month.args.arg0);
}

test(get_interval_shall_call_get_scheduled_day_correctly) {
  GET_INTERVAL_FIXTURE;
  assert_eq(1, m.get_scheduled_day.call_count);
  assert_eq(str, m.get_scheduled_day.args.arg0);
}

/*
 * story_init()
 */
test(story_init_shall_call_get_status_with_correct_arguments) {
  story_t story;

  story_init(1234, &story, (char*)5678, (char*)8765);

  assert_eq(1, m.get_status.call_count);
  assert_eq(1234, m.get_status.args.arg0);
  assert_eq((char*)5678, m.get_status.args.arg1);
}

test(story_init_shall_initialize_a_DONE_story_if_it_is_DONE) {
  story_t story;

  m.get_status.retval = STATUS_DONE;

  story_init(1234, &story, NULL, NULL);

  assert_eq(STATUS_DONE, story.status);
}

test(story_init_shall_initialize_a_TODO_story_if_it_is_TODO) {
  story_t story;

  m.get_status.retval = STATUS_TODO;

  story_init(1234, &story, NULL, NULL);

  assert_eq(STATUS_TODO, story.status);
}

test(story_init_shall_call_has_estimate_with_correct_arguments) {
  story_t story;

  story_init(1234, &story, (char*)5678, (char*)8765);

  assert_eq(1, m.has_estimate.call_count);
  assert_eq(1234, m.has_estimate.args.arg0);
  assert_eq((char*)5678, m.has_estimate.args.arg1);
}

#define STORY_INIT_FIXTURE_HAS_ESTIMATE                 \
  story_t story;                                        \
  m.has_estimate.retval = 1234;               \
  story_init(1, &story, (char*)5678, (char*)8765)

test(story_init_shall_call_has_estimate_range_with_correct_arguments) {
  STORY_INIT_FIXTURE_HAS_ESTIMATE;
  assert_eq(1, m.has_estimate_range.call_count);
  assert_eq(1234, m.has_estimate_range.args.arg0);
  assert_eq((char*)5678, m.has_estimate_range.args.arg1);
}

test(story_init_shall_initialize_a_story_with_an_estimate_if_no_range) {
  const int some_estimate = 32;

  m.get_estimate.retval = some_estimate;

  STORY_INIT_FIXTURE_HAS_ESTIMATE;

  assert_eq(ESTIMATE_POINTS, story.estimate_type);
  assert_eq(some_estimate, story.estimate.points);
}

#define STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE           \
  story_t story;                                        \
  m.has_estimate.retval = 1234;               \
  m.has_estimate_range.retval = 4321;         \
  story_init(1, &story, (char*)5678, (char*)8765);

test(story_init_shall_call_has_slogan_with_correct_arguments) {
  STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE;
  assert_eq(1, m.has_slogan.call_count);
  assert_eq(1, m.has_slogan.args.arg0);
  assert_eq(1234, m.has_slogan.args.arg1);
  assert_eq(4321, m.has_slogan.args.arg2);
  assert_eq((char*)5678, m.has_slogan.args.arg3);
}

test(story_init_shall_call_has_interval_with_correct_arguments) {
  STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE;
  assert_eq(1, m.has_interval.call_count);
  assert_eq(1, m.has_interval.args.arg0);
  assert_eq((char*)8765, m.has_interval.args.arg1);
}

test(story_init_shall_initialize_a_story_with_an_estimate_range_if_range) {
  const int some_estimate = 32;
  const int another_estimate = 64;

  m.get_estimate.retval = some_estimate;
  m.get_max_estimate.retval = another_estimate;

  STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE;

  assert_eq(ESTIMATE_RANGE, story.estimate_type);
  assert_eq(some_estimate, story.estimate.range.min_points);
  assert_eq(another_estimate, story.estimate.range.max_points);
}

#define STORY_INIT_FIXTURE_HAS_NO_ESTIMATE_RANGE        \
  story_t story;                                        \
  m.has_estimate.retval = 1234;               \
  m.has_estimate_range.retval = 0;            \
  story_init(1, &story, (char*)5678, (char*)8765);

test(story_init_shall_call_get_estimate_correct_if_no_range_is_specified) {
  STORY_INIT_FIXTURE_HAS_NO_ESTIMATE_RANGE;
  assert_eq(1, m.get_estimate.call_count);
  assert_eq(1234, m.get_estimate.args.arg0);
  assert_eq((char*)5678, m.get_estimate.args.arg1);
}

test(story_init_shall_not_call_get_max_estimate_if_no_range_is_specified) {
  STORY_INIT_FIXTURE_HAS_NO_ESTIMATE_RANGE;
  assert_eq(0, m.get_max_estimate.call_count);
}

test(story_init_shall_call_get_estimate_correct_if_range_is_specified) {
  STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE;
  assert_eq(1, m.get_estimate.call_count);
  assert_eq(1234, m.get_estimate.args.arg0);
  assert_eq((char*)5678, m.get_estimate.args.arg1);
}

test(story_init_shall_call_get_max_estimate_correctly_if_range_is_specified) {
  STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE;
  assert_eq(1, m.get_max_estimate.call_count);
  assert_eq(4321, m.get_max_estimate.args.arg0);
  assert_eq((char*)5678, m.get_max_estimate.args.arg1);
}

#define STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE_AND_SLOGAN   \
  story_t story;                                           \
  m.has_slogan.retval = 1234;                    \
  m.has_estimate.retval = 8765;                  \
  m.has_estimate_range.retval = 4321;

test(story_init_shall_call_get_slogan_length_correctly) {
  STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE_AND_SLOGAN;
  story_init(1, &story, (char*)8888, (char*)9999);
  assert_eq(1, m.get_slogan_length.call_count);
  assert_eq(1234, m.get_slogan_length.args.arg0);
  assert_eq(4321, m.get_slogan_length.args.arg1);
  assert_eq((char*)8888, m.get_slogan_length.args.arg2);
}

test(story_init_shall_malloc_the_correct_number_of_bytes_for_slogan) {
  STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE_AND_SLOGAN;

  m.get_slogan_length.retval = 13;
  m.malloc.retval = (void*)42; /* Avaid the out-of-memory assert*/

  story_init(1, &story, (char*)8888, (char*)9999);

  assert_eq(1, m.malloc.call_count);
  assert_eq(13, m.malloc.args.arg0);
}

test(story_init_shall_call_get_slogan_with_correct_arguments) {
  STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE_AND_SLOGAN;

  m.get_slogan_length.retval = 13;
  m.malloc.retval = (void*)42;

  story_init(1, &story, (char*)8888, (char*)9999);

  assert_eq((char*)42, m.get_slogan.args.arg0);
  assert_eq(4321, m.get_slogan.args.arg1);
  assert_eq(13, m.get_slogan.args.arg2);
  assert_eq((char*)8888, m.get_slogan.args.arg3);
}

test(story_init_shall_initialize_a_story_with_a_slogan_when_there_is_one) {
  STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE_AND_SLOGAN;

  m.get_slogan_length.retval = 13;
  m.malloc.retval = (void*)42;

  story_init(1, &story, (char*)8888, (char*)9999);

  /* No need to test anythong else than that malloc set the pointer where
   * the slogan is supposed to be copied to. */
  assert_eq((char*)42, story.slogan);
}

#define STORY_INIT_FIXTURE_HAS_INTERVAL                       \
  story_t story;                                              \
  m.get_status.retval = STATUS_DONE;                \
  m.has_interval.retval = 4321;

test(story_init_shall_call_get_interval_with_correct_arguments) {
  STORY_INIT_FIXTURE_HAS_INTERVAL;

  story_init(1, &story, (char*)8888, (char*)9999);

  assert_eq(&story.started, m.get_interval.args.arg0);
  assert_eq(&story.ended, m.get_interval.args.arg1);
  assert_eq((char*)9999, m.get_interval.args.arg2);
}

static void get_interval_stub(date_t* started, date_t* ended,
                              const char* str) {
  str = str;
  started->year = 1234;
  started->month = 56;
  started->day = 78;
  ended->year = 8765;
  ended->month = 43;
  ended->day = 21;
}

test(story_init_shall_initialize_a_story_with_an_interval_when_there_is_one) {
  STORY_INIT_FIXTURE_HAS_INTERVAL;

  m.get_interval.func = get_interval_stub;

  story_init(1, &story, (char*)8888, (char*)9999);

  assert_eq(1234, story.started.year);
  assert_eq(56, story.started.month);
  assert_eq(78, story.started.day);

  assert_eq(8765, story.ended.year);
  assert_eq(43, story.ended.month);
  assert_eq(21, story.ended.day);
}

/* TODO: Switch to module_test() macro when supported in cutest. */
test(story_should_init_shall_initialize_stories_correctly) {
  story_t story;
  char dest[1024];

  /* Integrate helpers by not mocking them */
  /* TODO: Remove these when module_test() macro is done in cutest. */
  m.strstr.func = strstr;
  m.strlen.func = strlen;
  m.strncmp.func = strncmp;
  m.memcpy.func = memcpy;

  m.get_status.func = get_status;
  m.has_estimate.func = has_estimate;
  m.has_estimate_range.func = has_estimate_range;
  m.has_slogan.func = has_slogan;
  m.get_estimate.func = get_estimate;
  m.get_max_estimate.func = get_max_estimate;
  m.get_slogan_length.func = get_slogan_length;
  m.get_slogan.func = get_slogan;

  m.is_date.retval = 1;
  m.is_date_formatted.func = is_date_formatted;
  m.has_interval.func = has_interval;
  m.has_date.func = has_date;
  m.has_scheduled.func = has_scheduled;
  m.has_deadline.func = has_deadline;
  m.get_interval.func = get_interval;
  m.get_year.func = get_year;
  m.get_month.func = get_month;
  m.get_day.func = get_day;
  m.get_scheduled_year.func = get_scheduled_year;
  m.get_scheduled_month.func = get_scheduled_month;
  m.get_scheduled_day.func = get_scheduled_day;
  m.get_deadline_year.func = get_deadline_year;
  m.get_deadline_month.func = get_deadline_month;
  m.get_deadline_day.func = get_deadline_day;

  /* But still mock the malloc to return a known data storage address */
  m.malloc.retval = &dest; /* No free(), hence no memoru leek */

  /* Top-level kick-the-tires test of is_story() */
  story_init(1, &story, "* TODO 01 This is the slogan 1", NULL);

  assert_eq(STATUS_TODO, story.status);
  assert_eq(ESTIMATE_POINTS, story.estimate_type);
  assert_eq(1, story.estimate.points);
  assert_eq(0, strcmp(story.slogan, "This is the slogan 1"));

  story_init(1, &story, "* TODO 01-03 This is the slogan 2", NULL);

  assert_eq(STATUS_TODO, story.status);
  assert_eq(ESTIMATE_RANGE, story.estimate_type);
  assert_eq(1, story.estimate.range.min_points);
  assert_eq(3, story.estimate.range.max_points);
  assert_eq(0, strcmp(story.slogan, "This is the slogan 2"));

  story_init(1, &story, "* DONE 02 This is the slogan 2",
             "  SCHEDULED: <2016-01-02> DEADLINE: <2017-03-04>");

  assert_eq(STATUS_DONE, story.status);
  assert_eq(2016, story.started.year);
  assert_eq(01, story.started.month);
  assert_eq(02, story.started.day);
  assert_eq(2017, story.ended.year);
  assert_eq(03, story.ended.month);
  assert_eq(04, story.ended.day);
}
