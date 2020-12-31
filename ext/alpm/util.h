#ifndef _ALPM_UTIL_H
#define _ALPM_UTIL_H

#include <ruby.h>
#include <string.h>
#include <alpm.h>

extern ID EXPLICIT;
extern ID DEPENDENCY;

VALUE alpm_list_to_rb_ary(alpm_list_t *lst, VALUE build(void *));

#endif
