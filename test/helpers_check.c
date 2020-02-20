#include <tarsio.h>
#include "sprint.h"
#include "helpers.h"
#include "helpers_data.h"

#define m tarsio_mock

/*
 * Renamed by pre-processor, calling it something more corelating
 * with the source code, not to hurt readers eyes (and brain) :D
 */
#define _isspace __ctype_b_loc

/*
 * rtrim()
 */
test(rtrim_shall_inplace_remove_whitespaces_at_end_of_the_string) {
  char str[32];

  m.strlen.func = strlen;
  m._isspace.func = _isspace;

  strcpy(str, "This is a string \t\r\n\n\t ");

  rtrim(str);

  assert_eq(0, strcmp(str, "This is a string"));
}

test(rtrim_shall_not_break_empty_string) {
  char str[32];

  m.strlen.func = strlen;
  m._isspace.func = _isspace;

  strcpy(str, "");

  rtrim(str);

  assert_eq(0, strcmp(str, ""));
}

#undef _isspace
