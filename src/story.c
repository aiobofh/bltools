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

  if (NULL == str) {
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

  if (NULL == str) {
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

static int has_tags(int orgmode_todo, const char* str) {
  size_t len;

  if (1 != orgmode_todo) {
    return 0;
  }

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

  if (0 == len) {
    return 0;
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

static int is_story(const char* str) {
  if (0 == call(has_orgmode_todo(str))) {
    return 0;
  }

  if (0 == call(has_estimate(1, str))) {
    return 0;
  }

  return 1;
}
