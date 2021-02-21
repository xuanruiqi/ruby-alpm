#ifndef _ALPM_DEPENDENCY_H
#define _ALPM_DEPENDENCY_H

#include <ruby.h>
#include <alpm.h>

#include "util.h"

extern const rb_data_type_t AlpmDependcy_type;
extern VALUE cDependency;

VALUE dependency_make(alpm_depend_t *dep);
void init_alpm_dependency(void);

#endif
