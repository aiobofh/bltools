#ifndef _BACKLOG_H_
#define _BACKLOG_H_

#include "story.h"

typedef int (*story_row_cb_t)(story_t* story);

int backlog_read(const char* filename, story_row_cb_t story_row_cb);

#endif
