#ifndef _ALPM_HANDLE_H
#define _ALPM_HANDLE_H

#include <ruby.h>
#include <string.h>
#include <alpm.h>

#include "util.h"

struct _AlpmHandle {
    alpm_handle_t *handle;
};
typedef struct _AlpmHandle AlpmHandle;

void Init_alpm_handle(void);

#endif
