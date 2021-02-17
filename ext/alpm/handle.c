#include <ruby.h>
#include <string.h>
#include <alpm.h>

#include "alpm_ruby.h"
#include "util.h"
#include "handle.h"

/* ctx is a double-pointer to alpm_handle_t */
static void handle_free(void *ctx)
{
    alpm_handle_t *handle = *((alpm_handle_t **) ctx);
    int res;
    
    if (handle) {
        res = alpm_release(handle);
        if (res != 0) {
            rb_raise(rb_eRuntimeError, "at __FILE__, __LINE__: Releasing handle failed");
        }
    }

    free(ctx);
}

static size_t handle_size(const void *ctx)
{
    return sizeof(*((alpm_handle_t **) ctx));
}

static const rb_data_type_t AlpmHandle_type = {
    .wrap_struct_name = "ALPM::Handle",
    .function = {
        .dmark = NULL,
        .dfree = handle_free,
        .dsize = handle_size,
    },
    .data = NULL,
    .flags = RUBY_TYPED_FREE_IMMEDIATELY,
};

static VALUE handle_allocate(VALUE self)
{
    alpm_handle_t **ctx;
    return TypedData_Make_Struct(self, alpm_handle_t*, &AlpmHandle_type, ctx);
}

static VALUE handle_c_initialize(VALUE self, VALUE root, VALUE dbpath)
{
    alpm_handle_t **ctx;
    char *root_c, *dbpath_c;
    alpm_errno_t err;
    const char *err_str;

    TypedData_Get_Struct(self, alpm_handle_t*, &AlpmHandle_type, ctx);
    
    Check_Type(root, T_STRING);
    Check_Type(dbpath, T_STRING);
    
    root_c = StringValueCStr(root);
    dbpath_c = StringValueCStr(dbpath);

    *ctx = alpm_initialize(root_c, dbpath_c, &err);
    if (err != 0) {
        err_str = alpm_strerror(err);
        rb_raise(rb_eRuntimeError, "Failed to initialize ALPM handle with error: %s, error code %d", err_str, err);
        return Qnil;
    }

    return self;
}

void init_alpm_handle(void)
{
    VALUE cHandle = rb_define_class_under(mALPM, "Handle", rb_cObject);

    rb_define_alloc_func(cHandle, handle_allocate);
    rb_define_method(cHandle, "initialize", handle_c_initialize, 2);
}
