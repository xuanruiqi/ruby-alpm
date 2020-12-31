#include <ruby.h>
#include <string.h>
#include <alpm.h>

#include "alpm_ruby.h"

VALUE mALPM;

VALUE libalpm_version(VALUE module) 
{
    const char *ver;
    ver = alpm_version();

    return rb_str_new_cstr(ver);
}

void Init_alpm(void)
{
    mALPM = rb_define_module("ALPM");    
    rb_define_singleton_method(mALPM, "libalpm_version", libalpm_version, 0);
}
