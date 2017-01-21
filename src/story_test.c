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
test(sum_min_estimates_shall_add_min_estimate_to_sum){
  int sum=4;
  story_t story;

  story.estimate_type=ESTIMATE_POINTS;
  story.estimate.points=1;
  sum_min_estimates(&sum,&story);
  assert_eq(5,sum);

}

test(sum_min_estimates_shall_add_min_estimate_from_range_to_sum){
  int sum=4;
  story_t story;

  story.estimate_type=ESTIMATE_RANGE;
  story.estimate.range.min_points=6;
  sum_min_estimates(&sum,&story);
  assert_eq(10,sum);
}

test(sum_max_estimates_shall_add_max_estimate_to_sum){
  int sum=20;
  story_t story;

  story.estimate_type = ESTIMATE_POINTS;
  story.estimate.points=30;
  sum_max_estimates(&sum,&story);
  assert_eq(50,sum);
}

test(sum_max_estimates_range_shall_add_max_estimate_from_range_to_sum){
  int sum=40;
  story_t story;

  story.estimate_type=ESTIMATE_RANGE;
  story.estimate.range.max_points=30;
  sum_max_estimates(&sum,&story);
  assert_eq(70,sum);
  
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
 * story_init()
 */
test(story_init_shall_call_get_status_with_correct_arguments) {
  story_t story;

  story_init(1234, &story, (char*)5678);

  assert_eq(1, cutest_mock.get_status.call_count);
  assert_eq(1234, cutest_mock.get_status.args.arg0);
  assert_eq((char*)5678, cutest_mock.get_status.args.arg1);
}

test(story_init_shall_initialize_a_DONE_story_if_it_is_DONE) {
  story_t story;

  cutest_mock.get_status.retval = STATUS_DONE;

  story_init(1234, &story, NULL);

  assert_eq(STATUS_DONE, story.status);
}

test(story_init_shall_initialize_a_TODO_story_if_it_is_TODO) {
  story_t story;

  cutest_mock.get_status.retval = STATUS_TODO;

  story_init(1234, &story, NULL);

  assert_eq(STATUS_TODO, story.status);
}

test(story_init_shall_has_estimate_with_correct_arguments) {
  story_t story;

  story_init(1234, &story, (char*)5678);

  assert_eq(1, cutest_mock.has_estimate.call_count);
  assert_eq(1234, cutest_mock.has_estimate.args.arg0);
  assert_eq((char*)5678, cutest_mock.has_estimate.args.arg1);
}

#define STORY_INIT_FIXTURE_HAS_ESTIMATE         \
  story_t story;                                \
  cutest_mock.has_estimate.retval = 8765;       \
  story_init(1, &story, (char*)5678)

test(story_init_shall_has_estimate_range_with_correct_arguments) {
  STORY_INIT_FIXTURE_HAS_ESTIMATE;
  assert_eq(1, cutest_mock.has_estimate_range.call_count);
  assert_eq(8765, cutest_mock.has_estimate_range.args.arg0);
  assert_eq((char*)5678, cutest_mock.has_estimate_range.args.arg1);
}

test(story_init_shall_initialize_a_story_with_an_estimate_if_no_range) {
  const int some_estimate = 32;

  cutest_mock.get_estimate.retval = some_estimate;

  STORY_INIT_FIXTURE_HAS_ESTIMATE;

  assert_eq(ESTIMATE_POINTS, story.estimate_type);
  assert_eq(some_estimate, story.estimate.points);
}

#define STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE   \
  story_t story;                                \
  cutest_mock.has_estimate.retval = 8765;       \
  cutest_mock.has_estimate_range.retval = 4321; \
  story_init(1, &story, (char*)5678);

test(story_init_shall_call_has_slogan_with_correct_arguments) {
  STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE;
  assert_eq(1, cutest_mock.has_slogan.call_count);
  assert_eq(1, cutest_mock.has_slogan.args.arg0);
  assert_eq(8765, cutest_mock.has_slogan.args.arg1);
  assert_eq(4321, cutest_mock.has_slogan.args.arg2);
  assert_eq((char*)5678, cutest_mock.has_slogan.args.arg3);
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
  cutest_mock.has_estimate.retval = 8765;               \
  cutest_mock.has_estimate_range.retval = 0;            \
  story_init(1, &story, (char*)5678);

test(story_init_shall_call_get_estimate_correct_if_no_range_is_specified) {
  STORY_INIT_FIXTURE_HAS_NO_ESTIMATE_RANGE;
  assert_eq(1, cutest_mock.get_estimate.call_count);
  assert_eq(8765, cutest_mock.get_estimate.args.arg0);
  assert_eq((char*)5678, cutest_mock.get_estimate.args.arg1);
}

test(story_init_shall_not_call_get_max_estimate_if_no_range_is_specified) {
  STORY_INIT_FIXTURE_HAS_NO_ESTIMATE_RANGE;
  assert_eq(0, cutest_mock.get_max_estimate.call_count);
}

test(story_init_shall_call_get_estimate_correct_if_range_is_specified) {
  STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE;
  assert_eq(1, cutest_mock.get_estimate.call_count);
  assert_eq(8765, cutest_mock.get_estimate.args.arg0);
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
  story_init(1, &story, (char*)5678);
  assert_eq(1, cutest_mock.get_slogan_length.call_count);
  assert_eq(1234, cutest_mock.get_slogan_length.args.arg0);
  assert_eq(4321, cutest_mock.get_slogan_length.args.arg1);
  assert_eq((char*)5678, cutest_mock.get_slogan_length.args.arg2);
}

test(story_init_shall_malloc_the_correct_number_of_bytes_for_slogan) {
  STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE_AND_SLOGAN;

  cutest_mock.get_slogan_length.retval = 13;
  cutest_mock.malloc.retval = (void*)42; /* Avaid the out-of-memory assert*/

  story_init(1, &story, (char*)5678);

  assert_eq(1, cutest_mock.malloc.call_count);
  assert_eq(13, cutest_mock.malloc.args.arg0);
}

test(story_init_shall_call_get_slogan_with_correct_arguments) {
  STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE_AND_SLOGAN;

  cutest_mock.get_slogan_length.retval = 13;
  cutest_mock.malloc.retval = (void*)42;

  story_init(1, &story, (char*)5678);

  assert_eq((char*)42, cutest_mock.get_slogan.args.arg0);
  assert_eq(4321, cutest_mock.get_slogan.args.arg1);
  assert_eq(13, cutest_mock.get_slogan.args.arg2);
  assert_eq((char*)5678, cutest_mock.get_slogan.args.arg3);
}

test(story_init_shall_initialize_a_story_with_a_slogan_when_there_is_one) {
  STORY_INIT_FIXTURE_HAS_ESTIMATE_RANGE_AND_SLOGAN;

  cutest_mock.get_slogan_length.retval = 13;
  cutest_mock.malloc.retval = (void*)42;

  story_init(1, &story, (char*)5678);

  /* No need to test anythong else than that malloc set the pointer where
   * the slogan is supposed to be copied to. */
  assert_eq((char*)42, story.slogan);
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

  /* But still mock the malloc to return a known data storage address */
  cutest_mock.malloc.retval = &dest; /* No free(), hence no memoru leek */

  /* Top-level kick-the-tires test of is_story() */
  story_init(1, &story, "* TODO 01 This is the slogan 1");

  assert_eq(STATUS_TODO, story.status);
  assert_eq(ESTIMATE_POINTS, story.estimate_type);
  assert_eq(1, story.estimate.points);
  assert_eq(0 == strcmp(story.slogan, "This is the slogan 1"));

  story_init(1, &story, "* TODO 01-03 This is the slogan 2");

  assert_eq(STATUS_TODO, story.status);
  assert_eq(ESTIMATE_RANGE, story.estimate_type);
  assert_eq(1, story.estimate.range.min_points);
  assert_eq(3, story.estimate.range.max_points);
  assert_eq(0 == strcmp(story.slogan, "This is the slogan 2"));
}
