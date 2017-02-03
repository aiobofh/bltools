#include "cutest.h"

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
  assert_eq(0, has_estimate(1, "* TODO Slogan "));
}

test(has_estimate_shall_return_0_if_there_is_no_space_or_dash_after) {
  assert_eq(0, has_estimate(1, "* TODO 03Slogan "));
}

test(has_estimate_shall_return_0_if_the_row_is_too_short) {
  assert_eq(0, has_estimate(1, "* TODO"));
}

test(has_estimate_shall_return_0_if_there_are_too_many_spaces) {
  assert_eq(0, has_estimate(1, "* TODO 01  Slogan"));
}

test(has_estimate_shall_return_1_if_the_row_has_an_estimate) {
  assert_eq(1, has_estimate(1, "* TODO 00 "));
  assert_eq(1, has_estimate(1, "* TODO 01 "));
  assert_eq(1, has_estimate(1, "* TODO 01-02"));
}

/*
 * has_estimate_range()
 */
test(has_estimate_range_shall_return_0_if_has_estimate_bool_is_false) {
  assert_eq(0, has_estimate_range(0, NULL));
}

test(has_estimate_range_shall_return_0_no_range_is_defined) {
  assert_eq(0, has_estimate_range(1, "* TODO 00 Slogan"));
}

test(has_estimate_range_shall_return_0_range_is_broken) {
  assert_eq(0, has_estimate_range(1, "* TODO 0001"));
}

test(has_estimate_range_shall_return_0_if_no_slogan_after_range) {
  assert_eq(0, has_estimate_range(1, "* TODO 00-01"));
}

test(has_estimate_range_shall_return_0_if_too_many_spaces) {
  assert_eq(0, has_estimate_range(1, "* TODO 00-01  "));
}

test(has_estimate_range_shall_return_1_if_range_is_defined) {
  assert_eq(1, has_estimate_range(1, "* TODO 00-01 Slogan"));
}

/*
 * has_tags()
 */
test(has_tags_shall_return_0_if_no_row_is_provided) {
  assert_eq(0, has_tags(1, NULL));
}

test(has_tags_shall_return_0_if_ogmode_todo_argument_is_false) {
  assert_eq(0, has_tags(0, NULL));
}

test(has_tags_shall_return_0_if_line_does_not_end_with_a_tag) {
  assert_eq(0, has_tags(1, "  Slogan"));
  assert_eq(0, has_tags(1, "  Slogan:"));
}

test(has_tags_shall_return_1_if_line_ends_with_a_tag) {
  assert_eq(1, has_tags(1, "  :tag:"));
}

/*
 * count_tags()
 */
test(count_tags_shall_return_0_if_no_row_is_provided) {
  assert_eq(0, count_tags(1, NULL));
}

test(count_tags_shall_return_0_if_tags_argument_is_false) {
  assert_eq(0, count_tags(0, NULL));
  assert_eq(0, count_tags(0, "bla bla bla :tag:"));
}

test(count_tags_shall_return_1_if_one_tag_is_specified) {
  assert_eq(1, count_tags(1, " :tag:"));
}

test(count_tags_shall_return_2_if_two_tags_are_specified) {
  assert_eq(2, count_tags(1, " :tag1:tag2:"));
}

test(count_tags_shall_return_5_if_five_tags_are_specified) {
  assert_eq(5, count_tags(1, " :tag1:tag2:tag3:tag4:tag5:"));
}

/*
 * is_date_formatted()
 */
test(is_date_formatted_shall_call_is_date_with_correct_argument) {
  assert_eq(0, is_date_formatted((char*)1234));
  assert_eq(1, cutest_mock.is_date.call_count);
  assert_eq((char*)1234, cutest_mock.is_date.args.arg0);
}

test(is_date_formatted_shall_return_0_if_date_suddanly_contains_lt) {
  cutest_mock.is_date.retval = 1;
  assert_eq(0, is_date_formatted("1234-12-12<"));
}

test(is_date_formatted_shall_return_0_if_does_not_end_with_gt_gt) {
  cutest_mock.is_date.retval = 1;
  assert_eq(0, is_date_formatted("1234-12-12"));
}

test(is_date_formatted_shall_return_1_if_string_start_with_date_and_gt) {
  cutest_mock.is_date.retval = 1;
  assert_eq(1, is_date_formatted("1234-12-12>"));
}

/*
 * has_date()
 */
test(has_date_shall_return_0_if_needle_is_not_found_in_haystack) {
  assert_eq(0, has_date("haystack", "needle"));
}

test(has_date_call_is_date_formatted_correctly_if_needle_is_found) {
  const char* str = "haystack needle haystack";
  assert_eq(0, has_date(str, "needle"));
  assert_eq(1, cutest_mock.is_date_formatted.call_count);
  assert_eq((char*)(str + 9 + strlen("needle")),
            cutest_mock.is_date_formatted.args.arg0);
}

/*
 * has_scheduled();
 */
test(has_scheduled_shall_call_has_date_correclty_and_forward_retval) {
  const char* str = "A silly string";

  cutest_mock.has_date.retval = 1234;

  assert_eq(1234, has_scheduled(str));
  assert_eq(str, cutest_mock.has_date.args.arg0);
  assert_eq(0 == strcmp("SCHEDULED: <", cutest_mock.has_date.args.arg1));
}

/*
 * has_deadline();
 */
test(has_deadline_shall_call_has_date_correclty_and_forward_retval) {
  const char* str = "A silly string";

  cutest_mock.has_date.retval = 1234;

  assert_eq(1234, has_deadline(str));
  assert_eq(str, cutest_mock.has_date.args.arg0);
  assert_eq(0 == strcmp("DEADLINE: <", cutest_mock.has_date.args.arg1));
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
  assert_eq(1, cutest_mock.has_scheduled.call_count);
  assert_eq((char*)5432, cutest_mock.has_scheduled.args.arg0);
}

test(has_interval_shall_call_has_deadline_correctly) {
  assert_eq(0, has_interval(1, (char*)5432));
  assert_eq(1, cutest_mock.has_deadline.call_count);
  assert_eq((char*)5432, cutest_mock.has_deadline.args.arg0);
}

test(has_interval_shall_return_0_if_no_scheduled_but_deadline) {
  cutest_mock.has_scheduled.retval = 0;
  cutest_mock.has_deadline.retval = 1;
  assert_eq(0, has_interval(1, (char*)5432));
}

test(has_interval_shall_return_0_if_no_deadline_but_scheduled) {
  cutest_mock.has_scheduled.retval = 1;
  cutest_mock.has_deadline.retval = 0;
  assert_eq(0, has_interval(1, (char*)5432));
}

/*
 * is_story()
 */
test(is_story_should_rely_on_helpers_since_they_are_already_tested) {
  const char *str = "Sould be passed to helpers";

  cutest_mock.has_orgmode_todo.retval = 1;
  cutest_mock.has_estimate.retval = 1;

  (void)is_story(str);

  /* has_orgmode_todo() */
  assert_eq(1, cutest_mock.has_orgmode_todo.call_count);
  assert_eq(0, strcmp(cutest_mock.has_orgmode_todo.args.arg0, str));

  /* has_estimate() */
  assert_eq(1, cutest_mock.has_estimate.call_count);
  assert_eq(1, cutest_mock.has_estimate.args.arg0);
  assert_eq(0, strcmp(cutest_mock.has_estimate.args.arg1, str));
}

test(is_story_should_return_0_if_has_orgmode_todo_fails) {
  const char *str = "Sould be passed to helpers";

  cutest_mock.has_orgmode_todo.retval = 0;

  assert_eq(0, is_story(str));
}

test(is_story_should_return_0_if_has_orgmode_todo_ok_but_has_estimate_fails) {
  const char *str = "Sould be passed to helpers";

  /* Note, this implies calling order is: has_orgmode_todo->has_estimate */
  cutest_mock.has_orgmode_todo.retval = 1;
  cutest_mock.has_estimate.retval = 0;

  assert_eq(0, is_story(str));
}

/* TODO: Switch to module_test() macro when supported in cutest. */
test(is_story_should_assess_stories_correctly) {
  /* Integrate helpers by not mocking them */
  /* TODO: Remove these when module_test() macro is done in cutest. */
  cutest_mock.has_orgmode_todo.func = has_orgmode_todo;
  cutest_mock.has_estimate.func = has_estimate;

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
  assert_eq(STATUS_TODO, get_status(1, "* TODO Slogan"));
}

test(get_status_should_return_STATUS_DONE_if_the_row_is_a_DONE_row) {
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
  get_slogan(dest, 0, 6, "* TODO 01 Slogan     :tag:");
  assert_eq(0 == strcmp(dest, "Slogan"));

  get_slogan(dest, 1, 6, "* TODO 01-12 SloGan     :tag:");
  assert_eq(0 == strcmp(dest, "SloGan"));

  get_slogan(dest, 0, 6, "* TODO 01 SloGAn");
  assert_eq(0 == strcmp(dest, "SloGAn"));
}

/*
 * get_year()
 */
test(get_year_shall_return_1234_if_needle_is_found_in_haystack) {
  assert_eq(1234, get_year("foo: <1234", "foo: <"));
}

/*
 * get_month()
 */
test(get_month_shall_return_56_if_needle_is_found_in_haystack) {
  assert_eq(56, get_month("foo: <1234-56", "foo: <"));
}

/*
 * get_day()
 */
test(get_month_shall_return_78_if_needle_is_found_in_haystack) {
  assert_eq(78, get_day("foo: <1234-56-78", "foo: <"));
}

/*
 * get_scheduled_year()
 */
test(get_scheduled_year_shall_call_get_year_correctly_and_forward_retval) {
  cutest_mock.get_year.retval = 1234;
  assert_eq(1234, get_scheduled_year("hobbla hobbla"));
  assert_eq(1, cutest_mock.get_year.call_count);
}

/*
 * get_scheduled_month()
 */
test(get_scheduled_month_shall_call_get_month_correctly_and_forward_retval) {
  cutest_mock.get_month.retval = 56;
  assert_eq(56, get_scheduled_month("hobbla hobbla"));
  assert_eq(1, cutest_mock.get_month.call_count);
}

/*
 * get_scheduled_day()
 */
test(get_scheduled_day_shall_call_get_day_correctly_and_forward_retval) {
  cutest_mock.get_day.retval = 78;
  assert_eq(78, get_scheduled_day("hobbla hobbla"));
  assert_eq(1, cutest_mock.get_day.call_count);
}

/*
 * get_deadline_year()
 */
test(get_deadline_year_shall_call_get_year_correctly_and_forward_retval) {
  cutest_mock.get_year.retval = 1234;
  assert_eq(1234, get_deadline_year("hobbla hobbla"));
  assert_eq(1, cutest_mock.get_year.call_count);
}

/*
 * get_deadline_month()
 */
test(get_deadline_month_shall_call_get_month_correctly_and_forward_retval) {
  cutest_mock.get_month.retval = 56;
  assert_eq(56, get_deadline_month("hobbla hobbla"));
  assert_eq(1, cutest_mock.get_month.call_count);
}

/*
 * get_deadline_day()
 */
test(get_deadline_day_shall_call_get_day_correctly_and_forward_retval) {
  cutest_mock.get_day.retval = 78;
  assert_eq(78, get_deadline_day("hobbla hobbla"));
  assert_eq(1, cutest_mock.get_day.call_count);
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
  assert_eq(1, cutest_mock.get_scheduled_year.call_count);
  assert_eq(str, cutest_mock.get_scheduled_year.args.arg0);
}

test(get_interval_shall_call_get_scheduled_month_correctly) {
  GET_INTERVAL_FIXTURE;
  assert_eq(1, cutest_mock.get_scheduled_month.call_count);
  assert_eq(str, cutest_mock.get_scheduled_month.args.arg0);
}

test(get_interval_shall_call_get_scheduled_day_correctly) {
  GET_INTERVAL_FIXTURE;
  assert_eq(1, cutest_mock.get_scheduled_day.call_count);
  assert_eq(str, cutest_mock.get_scheduled_day.args.arg0);
}

/*
 * story_init()
 */
test(story_init_shall_call_get_status_with_correct_arguments) {
  story_t story;

  story_init(1234, &story, (char*)5678, (char*)8765);

  assert_eq(1, cutest_mock.get_status.call_count);
  assert_eq(1234, cutest_mock.get_status.args.arg0);
  assert_eq((char*)5678, cutest_mock.get_status.args.arg1);
}

test(story_init_shall_initialize_a_DONE_story_if_it_is_DONE) {
  story_t story;

  cutest_mock.get_status.retval = STATUS_DONE;

  story_init(1234, &story, NULL, NULL);

  assert_eq(STATUS_DONE, story.status);
}

test(story_init_shall_initialize_a_TODO_story_if_it_is_TODO) {
  story_t story;

  cutest_mock.get_status.retval = STATUS_TODO;

  story_init(1234, &story, NULL, NULL);

  assert_eq(STATUS_TODO, story.status);
}

test(story_init_shall_call_has_estimate_with_correct_arguments) {
  story_t story;

  story_init(1234, &story, (char*)5678, (char*)8765);

  assert_eq(1, cutest_mock.has_estimate.call_count);
  assert_eq(1234, cutest_mock.has_estimate.args.arg0);
  assert_eq((char*)5678, cutest_mock.has_estimate.args.arg1);
}

#define STORY_INIT_FIXTURE_HAS_ESTIMATE                 \
  story_t story;                                        \
  cutest_mock.has_estimate.retval = 1234;               \
  story_init(1, &story, (char*)5678, (char*)8765)

test(story_init_shall_call_has_estimate_range_with_correct_arguments) {
  STORY_INIT_FIXTURE_HAS_ESTIMATE;
  assert_eq(1, cutest_mock.has_estimate_range.call_count);
  assert_eq(1234, cutest_mock.has_estimate_range.args.arg0);
  assert_eq((char*)5678, cutest_mock.has_estimate_range.args.arg1);
}

test(story_init_shall_initialize_a_story_with_an_estimate_if_no_range) {
  const int some_estimate = 32;

  cutest_mock.get_estimate.retval = some_estimate;

  STORY_INIT_FIXTURE_HAS_ESTIMATE;

  assert_eq(ESTIMATE_POINTS, story.estimate_type);
  assert_eq(some_estimate, story.estimate.points);
}

#define STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE           \
  story_t story;                                        \
  cutest_mock.has_estimate.retval = 1234;               \
  cutest_mock.has_estimate_range.retval = 4321;         \
  story_init(1, &story, (char*)5678, (char*)8765);

test(story_init_shall_call_has_slogan_with_correct_arguments) {
  STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE;
  assert_eq(1, cutest_mock.has_slogan.call_count);
  assert_eq(1, cutest_mock.has_slogan.args.arg0);
  assert_eq(1234, cutest_mock.has_slogan.args.arg1);
  assert_eq(4321, cutest_mock.has_slogan.args.arg2);
  assert_eq((char*)5678, cutest_mock.has_slogan.args.arg3);
}

test(story_init_shall_call_has_interval_with_correct_arguments) {
  STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE;
  assert_eq(1, cutest_mock.has_interval.call_count);
  assert_eq(1, cutest_mock.has_interval.args.arg0);
  assert_eq((char*)8765, cutest_mock.has_interval.args.arg1);
}

test(story_init_shall_initialize_a_story_with_an_estimate_range_if_range) {
  const int some_estimate = 32;
  const int another_estimate = 64;

  cutest_mock.get_estimate.retval = some_estimate;
  cutest_mock.get_max_estimate.retval = another_estimate;

  STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE;

  assert_eq(ESTIMATE_RANGE, story.estimate_type);
  assert_eq(some_estimate, story.estimate.range.min_points);
  assert_eq(another_estimate, story.estimate.range.max_points);
}

#define STORY_INIT_FIXTURE_HAS_NO_ESTIMATE_RANGE        \
  story_t story;                                        \
  cutest_mock.has_estimate.retval = 1234;               \
  cutest_mock.has_estimate_range.retval = 0;            \
  story_init(1, &story, (char*)5678, (char*)8765);

test(story_init_shall_call_get_estimate_correct_if_no_range_is_specified) {
  STORY_INIT_FIXTURE_HAS_NO_ESTIMATE_RANGE;
  assert_eq(1, cutest_mock.get_estimate.call_count);
  assert_eq(1234, cutest_mock.get_estimate.args.arg0);
  assert_eq((char*)5678, cutest_mock.get_estimate.args.arg1);
}

test(story_init_shall_not_call_get_max_estimate_if_no_range_is_specified) {
  STORY_INIT_FIXTURE_HAS_NO_ESTIMATE_RANGE;
  assert_eq(0, cutest_mock.get_max_estimate.call_count);
}

test(story_init_shall_call_get_estimate_correct_if_range_is_specified) {
  STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE;
  assert_eq(1, cutest_mock.get_estimate.call_count);
  assert_eq(1234, cutest_mock.get_estimate.args.arg0);
  assert_eq((char*)5678, cutest_mock.get_estimate.args.arg1);
}

test(story_init_shall_call_get_max_estimate_correctly_if_range_is_specified) {
  STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE;
  assert_eq(1, cutest_mock.get_max_estimate.call_count);
  assert_eq(4321, cutest_mock.get_max_estimate.args.arg0);
  assert_eq((char*)5678, cutest_mock.get_max_estimate.args.arg1);
}

#define STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE_AND_SLOGAN   \
  story_t story;                                           \
  cutest_mock.has_slogan.retval = 1234;                    \
  cutest_mock.has_estimate.retval = 8765;                  \
  cutest_mock.has_estimate_range.retval = 4321;

test(story_init_shall_call_get_slogan_length_correctly) {
  STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE_AND_SLOGAN;
  story_init(1, &story, (char*)8888, (char*)9999);
  assert_eq(1, cutest_mock.get_slogan_length.call_count);
  assert_eq(1234, cutest_mock.get_slogan_length.args.arg0);
  assert_eq(4321, cutest_mock.get_slogan_length.args.arg1);
  assert_eq((char*)8888, cutest_mock.get_slogan_length.args.arg2);
}

test(story_init_shall_malloc_the_correct_number_of_bytes_for_slogan) {
  STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE_AND_SLOGAN;

  cutest_mock.get_slogan_length.retval = 13;
  cutest_mock.malloc.retval = (void*)42; /* Avaid the out-of-memory assert*/

  story_init(1, &story, (char*)8888, (char*)9999);

  assert_eq(1, cutest_mock.malloc.call_count);
  assert_eq(13, cutest_mock.malloc.args.arg0);
}

test(story_init_shall_call_get_slogan_with_correct_arguments) {
  STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE_AND_SLOGAN;

  cutest_mock.get_slogan_length.retval = 13;
  cutest_mock.malloc.retval = (void*)42;

  story_init(1, &story, (char*)8888, (char*)9999);

  assert_eq((char*)42, cutest_mock.get_slogan.args.arg0);
  assert_eq(4321, cutest_mock.get_slogan.args.arg1);
  assert_eq(13, cutest_mock.get_slogan.args.arg2);
  assert_eq((char*)8888, cutest_mock.get_slogan.args.arg3);
}

test(story_init_shall_initialize_a_story_with_a_slogan_when_there_is_one) {
  STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE_AND_SLOGAN;

  cutest_mock.get_slogan_length.retval = 13;
  cutest_mock.malloc.retval = (void*)42;

  story_init(1, &story, (char*)8888, (char*)9999);

  /* No need to test anythong else than that malloc set the pointer where
   * the slogan is supposed to be copied to. */
  assert_eq((char*)42, story.slogan);
}

#define STORY_INIT_FIXTURE_HAS_INTERVAL                       \
  story_t story;                                              \
  cutest_mock.get_status.retval = STATUS_DONE;                \
  cutest_mock.has_interval.retval = 4321;

test(story_init_shall_call_get_interval_with_correct_arguments) {
  STORY_INIT_FIXTURE_HAS_INTERVAL;

  story_init(1, &story, (char*)8888, (char*)9999);

  assert_eq(&story.started, cutest_mock.get_interval.args.arg0);
  assert_eq(&story.ended, cutest_mock.get_interval.args.arg1);
  assert_eq((char*)9999, cutest_mock.get_interval.args.arg2);
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

  cutest_mock.get_interval.func = get_interval_stub;

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
  cutest_mock.get_status.func = get_status;
  cutest_mock.has_estimate.func = has_estimate;
  cutest_mock.has_estimate_range.func = has_estimate_range;
  cutest_mock.has_slogan.func = has_slogan;
  cutest_mock.get_estimate.func = get_estimate;
  cutest_mock.get_max_estimate.func = get_max_estimate;
  cutest_mock.get_slogan_length.func = get_slogan_length;
  cutest_mock.get_slogan.func = get_slogan;

  cutest_mock.is_date.retval = 1;
  cutest_mock.is_date_formatted.func = is_date_formatted;
  cutest_mock.has_interval.func = has_interval;
  cutest_mock.has_date.func = has_date;
  cutest_mock.has_scheduled.func = has_scheduled;
  cutest_mock.has_deadline.func = has_deadline;
  cutest_mock.get_interval.func = get_interval;
  cutest_mock.get_year.func = get_year;
  cutest_mock.get_month.func = get_month;
  cutest_mock.get_day.func = get_day;
  cutest_mock.get_scheduled_year.func = get_scheduled_year;
  cutest_mock.get_scheduled_month.func = get_scheduled_month;
  cutest_mock.get_scheduled_day.func = get_scheduled_day;
  cutest_mock.get_deadline_year.func = get_deadline_year;
  cutest_mock.get_deadline_month.func = get_deadline_month;
  cutest_mock.get_deadline_day.func = get_deadline_day;

  /* But still mock the malloc to return a known data storage address */
  cutest_mock.malloc.retval = &dest; /* No free(), hence no memoru leek */

  /* Top-level kick-the-tires test of is_story() */
  story_init(1, &story, "* TODO 01 This is the slogan 1", NULL);

  assert_eq(STATUS_TODO, story.status);
  assert_eq(ESTIMATE_POINTS, story.estimate_type);
  assert_eq(1, story.estimate.points);
  assert_eq(0 == strcmp(story.slogan, "This is the slogan 1"));

  story_init(1, &story, "* TODO 01-03 This is the slogan 2", NULL);

  assert_eq(STATUS_TODO, story.status);
  assert_eq(ESTIMATE_RANGE, story.estimate_type);
  assert_eq(1, story.estimate.range.min_points);
  assert_eq(3, story.estimate.range.max_points);
  assert_eq(0 == strcmp(story.slogan, "This is the slogan 2"));

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
