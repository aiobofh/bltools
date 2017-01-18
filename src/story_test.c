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

test(is_story_should_assess_stories_correctly) {
  /* Integrate helpers by not mocking them */
  cutest_mock.has_orgmode_todo.func = has_orgmode_todo;
  cutest_mock.has_estimate.func = has_estimate;

  /* Top-level test of is_story() */
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
