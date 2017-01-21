
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "story.h"

static int has_orgmode_todo(const char* str) {
  const char *s = &str[2];

  if (NULL == str) {
    return 0;
  }

  /* Every org-mode TODO/DONE item start with '*' */
  if ('*' != str[0]) {
    return 0;
  }

  /* ... Followed by a space */
  if (' ' != str[1]) {
    return 0;
  }

  /* ... Then contain either 'TODO' or 'DONE' followed by a space */
  if ((0 != strncmp(s, "TODO ", 5)) && (0 != strncmp(s, "DONE ", 5))) {
    return 0;
  }

  /* ... Then a character other than space. */
  if (' ' == s[5]) {
    return 0;
  }

  return 1;
}

static int has_estimate(int orgmode_todo, const char* str) {
  const size_t len = strlen("* SSSS ");
  const char *s = &str[len];

  if (1 != orgmode_todo) {
    return 0;
  }

  if (strlen(str) < len) {
    return 0;
  }

  /* An estimate must start with two digits (leading zero) */
  if (!(('0' <= s[0]) && ('9' >= s[0]) && ('0' <= s[1]) && ('9' >= s[1]))) {
    return 0;
  }

  /* ... Followed by space or a dash */
  if ((' ' != s[2]) && ('-' != s[2])) {
    return 0;
  }

  /* .. And only one space (if single estimate) */
  if ((' ' == s[3])) {
    return 0;
  }

  return 1;
}

static int has_estimate_range(int estimate, const char* str) {
  const size_t len = strlen("* SSSS 00");
  const char *s = &str[len];

  if (1 != estimate) {
    return 0;
  }

  /* An estimate range must start with a '-' after the first estimate. */
  if ((' ' != s[0]) && ('-' != s[0])) {
    return 0;
  }

  /* ... Followed by two digits (leading zero) */
  if (!(('0' <= s[1]) && ('9' >= s[1]) && ('0' <= s[2]) && ('9' >= s[2]))) {
    return 0;
  }

  /* ... And a space before the slogan */
  if (!(' ' == s[3])) {
    return 0;
  }

  /* .. And only one space */
  if ((' ' == s[4])) {
    return 0;
  }

  return 1;
}


static int has_slogan(int orgmode_todo, int estimate, int estimate_range, const char *str) {
  int offset = 10;

  /* It must be an org-mode TODO item */
  if (0 == orgmode_todo) {
    return 0;
  }

  /* It must have an estimate */
  if (0 == estimate) {
    return 0;
  }

  /* It could have an estiamte range */
  if (1 == estimate_range) {
    offset = 13;
  }

  /* It should not be a ' ' */
  if (' ' == str[offset + 1]) {
    return 0;
  }

  /* ... Nor just a tag */
  if (':' == str[offset + 1]) {
    return 0;
  }

  return 1;
}

static int get_slogan_length(int slogan, int estimate_range, const char* str) {
  const int len = strlen(str);
  int l = 0;
  int offset = 10;

  /* It must be a slogan */
  if (0 == slogan) {
    return 0;
  }

  /* And might have an estimate range */
  if (1 == estimate_range) {
    offset = 13;
  }

  /* Count charactesr until we reach a tag or the end of the string */
  while ((':' != str[offset]) && (offset < len)) {
    offset++;
    l++;
  }

  /* Reverse to the first non-space character */
  offset--;
  while (' ' == str[offset]) {
    offset--;
    l--;
  }

  return l;
}

void get_slogan(char* dest, int estimate_range, int slogan_length, const char* str) {
  int offset = 10;

  assert(0 != slogan_length && "The slogan must have a range");

  /* The story might have an estimate-range */
  if (1 == estimate_range) {
    offset = 13;
  }

  /* Copy the slonan-part of the string */
  memcpy(dest, &str[offset], slogan_length);

  /* And null-terminate it */
  dest[slogan_length] = '\0';
}

static int has_tags(int orgmode_todo, const char* str) {
  size_t len;

  /* It must be an org-mode item */
  if (1 != orgmode_todo) {
    return 0;
  }

  /* And have a string */
  if (NULL == str) {
    return 0;
  }

  len = strlen(str) - 1;

  /* A row with org-mode tags must end with a ':' */
  if (':' != str[len]) {
    return 0;
  }

  /* .. And have a single word, prefixed by the first ':' */
  while (len-- > 0) {
    if (' ' == str[len]) {
      return 0;
    }
    if (':' == str[len]) {
      break;
    }
  }

  return 1;
}

static int count_tags(int tags, const char* str) {
  int cnt = 0;
  size_t len;

  if (1 != tags) {
    return 0;
  }

  if (NULL == str) {
    return 0;
  }

  len = strlen(str) - 1;

  /* Every tag must start, and end with ':' at the end of the row */
  while (len-- > 0) {
    if (' ' == str[len]) {
      break;
    }
    if (':' == str[len]) {
      cnt++;
    }
  }

  return cnt;
}

int is_story(const char* str) {
  if (0 == call(has_orgmode_todo(str))) {
    return 0;
  }

  if (0 == call(has_estimate(1, str))) {
    return 0;
  }

  return 1;
}

static status_t get_status(int story, const char* str) {
  assert(1 == story && "is_story() must have returned 1 before calling");
  story = story;
  if (0 == strncmp(&str[2], "TODO", 4)) {
    return STATUS_TODO;
  }
  return STATUS_DONE;
}

static int get_estimate(int estimate, const char* str) {
  assert(1 == estimate && "had_estimate() must have returned 1 before calling");
  estimate = estimate;
  return (str[7] - '0') * 10 + (str[8] - '0');
}

static int get_max_estimate(int estimate_range, const char* str) {
  assert(1 == estimate_range && "had_estimate_range() must have returned 1 before calling");
  estimate_range = estimate_range;
  return (str[10] - '0') * 10 + (str[11] - '0');
}

void sum_min_estimates(int *sum, story_t *story) {
  assert(story->estimate.points == story->estimate.range.min_points &&
	 "sum_min_estimates assumes union is gcc-aligned");
  *sum += story->estimate.points;
}

void sum_max_estimates(int *sum, story_t *story) {
  if (ESTIMATE_RANGE == story->estimate_type) {
    *sum += story->estimate.range.max_points;
  }
  else {
    *sum += story->estimate.points;
  }
}

void story_init(int story, story_t* s, const char* str) {
  assert(1 == story && "is_story() must have returned 1 before calling");

  /* Extract the status */
  s->status = call(get_status(story, str));

  const int estimate = call(has_estimate(story, str));
  const int estimate_range = call(has_estimate_range(estimate, str));
  const int slogan = call(has_slogan(1, estimate, estimate_range, str));

  /* Extract the estimate */
  if (0 != estimate_range) {
    s->estimate_type = ESTIMATE_RANGE;
    s->estimate.range.min_points = call(get_estimate(estimate, str));
    s->estimate.range.max_points = call(get_max_estimate(estimate_range, str));
  }
  else {
    s->estimate_type = ESTIMATE_POINTS;
    s->estimate.points = call(get_estimate(estimate, str));
  }

  if (0 != slogan) {
    const int length = call(get_slogan_length(slogan, estimate_range, str));
    s->slogan = call(malloc(length));
    assert(NULL != s->slogan && "Out of memory");
    call(get_slogan(s->slogan, estimate_range, length, str));
  }
}
