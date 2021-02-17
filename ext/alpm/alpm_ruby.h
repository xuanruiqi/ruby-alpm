#ifndef _ALPM_H
#define _ALPM_H

#include <ruby.h>

extern VALUE mALPM;
VALUE libalpm_version(VALUE module);
void Init_alpm(void);

#endif
