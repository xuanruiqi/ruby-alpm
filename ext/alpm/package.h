#ifndef _ALPM_PACKAGE_H
#define _ALPM_PACKAGE_H

#include <ruby.h>
#include <string.h>
#include <alpm.h>

#include "util.h"

typedef struct _AlpmPkg {
    alpm_pkg_t *pkg;
} AlpmPkg;

void Init_alpm_package(void);

#endif
