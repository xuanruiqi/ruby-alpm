#ifndef _ALPM_PACKAGE_H
#define _ALPM_PACKAGE_H

#include <ruby.h>
#include <string.h>
#include <alpm.h>

#include "util.h"

extern const rb_data_type_t AlpmPkg_type;
extern VALUE cPackage;

void init_alpm_package(void);

#endif
