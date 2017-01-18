#include <string.h>

#include "story.h"

static int has_orgmode_todo(const char* str) {
  const char *s = &str[2];
  if (NULL == str) {
    return 0;
  }
  if ('*' != str[0]) {
    return 0;
  }
  if (' ' != str[1]) {
    return 0;
  }
  if ((0 != strncmp(s, "TODO ", 5)) && (0 != strncmp(s, "DONE ", 5))) {
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
  if (!(('0' <= s[0]) && ('9' >= s[0]) && ('0' <= s[1]) && ('9' >= s[1]))) {
    return 0;
  }
  if ((' ' != s[2]) && ('-' != s[2])) {
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
  if ((' ' != s[0]) && ('-' != s[0])) {
    return 0;
  }
  if (!(('0' <= s[1]) && ('9' >= s[1]) && ('0' <= s[2]) && ('9' >= s[2]))) {
    return 0;
  }
  if (!(' ' == s[3])) {
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
  if (':' != str[len]) {
    return 0;
  }
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
