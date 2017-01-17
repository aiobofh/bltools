#include "cutest.h"

test(has_orgmode_todo_shall_return_true_if_row_contains_todo_or_done) {
  assert_eq(1, has_orgmode_todo("* TODO "));
  assert_eq(1, has_orgmode_todo("* DONE "));
}

test(has_orgmode_todo_shall_return_false_if_row_does_not_start_with_star) {
  assert_eq(0, has_orgmode_todo(" * TODO "));
  assert_eq(0, has_orgmode_todo("# DONE "));
}

test(has_orgmode_todo_shall_return_false_if_length_is_too_short) {
  assert_eq(0, has_orgmode_todo("*"));
}

test(has_orgmode_todo_shall_return_false_if_row_dows_not_contain_todo_or_done) {
  assert_eq(0, has_orgmode_todo("* Slogan"));
}

test(has_estimate_shall_return_false_if_ogmode_todo_argument_is_false) {
  assert_eq(0, has_estimate(0, "* TODO 00 "));
}

test(has_estimate_shall_return_true_if_the_row_has_an_estimate) {
  assert_eq(1, has_estimate(1, "* TODO 00 "));
  assert_eq(1, has_estimate(1, "* TODO 01 "));
  assert_eq(1, has_estimate(1, "* TODO 01-02"));
}

test(has_estimate_shall_return_false_if_the_row_has_no_estimate) {
  assert_eq(0, has_estimate(1, "* TODO Slogan "));
}

test(has_estimate_shall_return_false_if_the_row_is_too_short) {
  assert_eq(0, has_estimate(1, "* TODO"));
}

test(has_estimate_range_shall_return_false_if_has_estimate_bool_is_false) {
  assert_eq(0, has_estimate_range(0, NULL));
}

test(has_estimate_range_shall_return_false_no_range_is_defined) {
  assert_eq(0, has_estimate_range(1, "* TODO 00 Slogan"));
}

test(has_estimate_range_shall_return_false_range_is_broken) {
  assert_eq(0, has_estimate_range(1, "* TODO 0001"));
}

test(has_estimate_range_shall_return_false_if_no_slogan_after_range) {
  assert_eq(0, has_estimate_range(1, "* TODO 00-01"));
}

test(has_estimate_range_shall_return_true_if_range_is_defined) {
  assert_eq(1, has_estimate_range(1, "* TODO 00-01 Slogan"));
}

test(has_tags_shall_return_false_if_ogmode_todo_argument_is_false) {
  assert_eq(0, has_tags(0, NULL));
}

test(has_tags_shall_return_false_if_no_row_is_provided) {
  assert_eq(0, has_tags(1, NULL));
}

test(has_tags_shall_return_false_if_line_does_not_end_with_a_tag) {
  assert_eq(0, has_tags(1, "  Slogan"));
  assert_eq(0, has_tags(1, "  Slogan:"));
}

test(has_tags_shall_return_true_if_line_ends_with_a_tag) {
  assert_eq(1, has_tags(1, "  :tag:"));
}

test(count_tags_shall_return_false_if_tags_argument_is_false) {
  assert_eq(0, count_tags(0, NULL));
}

test(count_tags_shall_return_false_if_no_row_is_provided) {
  assert_eq(0, count_tags(1, NULL));
}

test(count_tags_shall_return_1_if_one_tag_is_specified) {
  assert_eq(1, count_tags(1, " :tag:"));
}

test(count_tags_shall_return_2_if_two_tags_are_specified) {
  assert_eq(2, count_tags(1, " :tag1:tag2:"));
}

test(count_tags_shall_return_much_if_many_tags_are_specified) {
  assert_eq(5, count_tags(1, " :tag1:tag2:tag3:tag4:tag5:"));
}
