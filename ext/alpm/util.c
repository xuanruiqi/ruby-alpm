#include <ruby.h>
#include <string.h>
#include <alpm.h>
#include <alpm_list.h>

#include "util.h"

VALUE alpm_list_to_rb_ary(alpm_list_t *lst, VALUE build(void *))
{
    VALUE arr = rb_ary_new();

    for (alpm_list_t *p = lst; p; p = alpm_list_next(p)) {
        VALUE dat = build(p->data);
        rb_ary_push(arr, dat);
    }

    return arr;
}
