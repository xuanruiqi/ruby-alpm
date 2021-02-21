#include <ruby.h>
#include <string.h>
#include <alpm.h>
#include <alpm_list.h>

#include "alpm_ruby.h"
#include "util.h"
#include "dependency.h"

VALUE cDependency;

static void dependency_free(void *ctx)
{
    alpm_depend_t *dep = *((alpm_depend_t **) ctx);

    if (pkg) {
        alpm_dep_free(dep);
    }
}

static size_t dependency_size(const void *ctx)
{
    return sizeof(*((alpm_depend_t **) ctx));
}

const rb_data_type_t AlpmDependency_type = {
    .wrap_struct_name = "ALPM::Dependency",
    .function = {
        .dmark = NULL,
        .dfree = dependency_free,
        .dsize = dependency_size
    },
    .data = NULL,
    .flags = RUBY_TYPED_FREE_IMMEDIATELY,
}

static VALUE dependency_allocate(VALUE self)
{
    alpm_depend_t **ctx;
    return TypedData_Make_Struct(self, alpm_depend_t*, &AlpmDependency_type, ctx);
}

/* Dependency.to_str : String */
static VALUE dependency_to_str(VALUE self)
{
    alpm_depend_t **ctx;
    char *dep_str;

    TypedData_Get_Struct(self, alpm_depend_t*, &AlpmDependency_type, ctx);

    dep_str = alpm_dep_compute_string(*ctx);
    return rb_str_new_cstr(dep_str);
}

VALUE dependency_make(alpm_depend_t *dep)
{
    alpm_depend_t **ctx;
    return TypedData_Make_Struct(cDependency, alpm_depend_t*, &AlpmDependency_type, ctx);
}

void init_alpm_dependency(void)
{
    cDependency = rb_define_class_under(mALPM, "Dependency", rb_cObject);

    rb_define_alloc_func(cDependency, dependency_allocate);
    rb_define_method(cDependency, "to_str", dependency_to_str, 0);
}
