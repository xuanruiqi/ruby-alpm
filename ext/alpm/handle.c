#include <ruby.h>
#include <string.h>
#include <alpm.h>

#include "alpm_ruby.h"
#include "util.h"
#include "handle.h"
#include "package.h"

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

/* Handle::new(root, dbpath) */
static VALUE handle_c_initialize(VALUE self, VALUE root, VALUE dbpath)
{
    alpm_handle_t **ctx;
    char *root_c, *dbpath_c;
    alpm_errno_t err;

    TypedData_Get_Struct(self, alpm_handle_t*, &AlpmHandle_type, ctx);
    
    Check_Type(root, T_STRING);
    Check_Type(dbpath, T_STRING);
    
    root_c = StringValueCStr(root);
    dbpath_c = StringValueCStr(dbpath);

    *ctx = alpm_initialize(root_c, dbpath_c, &err);
    if (err != 0) {
        rb_raise(rb_eRuntimeError, "Failed to initialize ALPM handle with error: %s, error code %d", alpm_strerror(err), err);
        return Qnil;
    }

    return self;
}

/* Handle::load_package(filename, sig_level = 1) */
VALUE package_load(int argc, VALUE *argv, VALUE self)
{
    alpm_handle_t **ctx;
    VALUE filename, sig_level, pkg_rb;
    alpm_pkg_t *pkg;
    alpm_pkg_t **pkg_ctx;
    char *filename_c;
    int sig_level_c, load_result;

    rb_scan_args(argc, argv, "11", &filename, &sig_level);
    
    if (!NIL_P(sig_level)) {
        sig_level_c = FIX2INT(sig_level);
    } else {
        sig_level_c = 1;
    }
    
    filename_c = StringValueCStr(filename);

    TypedData_Get_Struct(self, alpm_handle_t*, &AlpmHandle_type, ctx);

    load_result = alpm_pkg_load(*ctx, filename_c, 1, sig_level_c, &pkg);

    if (load_result == -1 || !pkg) {
        rb_raise(rb_eRuntimeError, "Failed to create package with error: %s", alpm_strerror(alpm_errno(*ctx)));
        return Qnil;
    }

    pkg_ctx = &pkg;
    pkg_rb = TypedData_Make_Struct(cPackage, alpm_pkg_t*, &AlpmPkg_type, pkg_ctx);
    return pkg_rb;
} 

void init_alpm_handle(void)
{
    VALUE cHandle = rb_define_class_under(mALPM, "Handle", rb_cObject);

    rb_define_alloc_func(cHandle, handle_allocate);
    rb_define_method(cHandle, "initialize", handle_c_initialize, 2);
}
