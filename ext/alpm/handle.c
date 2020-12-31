#include <ruby.h>
#include <string.h>
#include <alpm.h>

#include "alpm_ruby.h"
#include "util.h"
#include "handle.h"

static void handle_free(void *ctx)
{
    alpm_handle_t *handle = (((AlpmHandle*)ctx)->handle);
    int res;
    
    if (handle) {
        res = alpm_release(handle);
        if (res != 0) {
            rb_raise(rb_eRuntimeError, "Releasing handle failed");
        }
    }

    free(ctx);
}

static const rb_data_type_t AlpmHandle_type = {
    .wrap_struct_name = "ALPM::Handle",
    .function = {
        .dmark = NULL,
        .dfree = handle_free,
        .dsize = NULL,
    },
    .data = NULL,
    .flags = RUBY_TYPED_FREE_IMMEDIATELY,
};

static VALUE handle_allocate(VALUE self)
{
    AlpmHandle *ctx;
    return TypedData_Make_Struct(self, AlpmHandle, &AlpmHandle_type, ctx);
}

static VALUE handle_c_initialize(VALUE self, VALUE root, VALUE dbpath)
{
    AlpmHandle *ctx;
    char *root_c, *dbpath_c;
    alpm_errno_t err;

    TypedData_Get_Struct(self, AlpmHandle, &AlpmHandle_type, ctx);
    
    Check_Type(root, T_STRING);
    Check_Type(dbpath, T_STRING);
    
    root_c = StringValueCStr(root);
    dbpath_c = StringValueCStr(dbpath);

    ctx->handle = alpm_initialize(root_c, dbpath_c, &err);
    if (err != 0) {
        rb_raise(rb_eRuntimeError, "Failed to initialize ALPM handle!");
        return Qnil;
    }

    return self;
}
