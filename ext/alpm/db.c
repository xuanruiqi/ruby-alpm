#include <ruby.h>
#include <alpm.h>
#include <stdbool.h>

#include "alpm_ruby.h"
#include "util.h"
#include "db.h"

static void db_free(void *ctx)
{
    free(ctx);
}

static size_t db_size(const void *ctx)
{
    (void) ctx;
    return sizeof(AlpmDB);
}

static const rb_data_type_t AlpmDB_type = {
    .wrap_struct_name = "ALPM::DB",
    .function = {
        .dmark = NULL,
        .dfree = db_free,
        .dsize = db_size,
    },
    .data = NULL,
    .flags = RUBY_TYPED_FREE_IMMEDIATELY,
};

static VALUE db_allocate(VALUE self)
{
    AlpmDB *ctx;
    return TypedData_Make_Struct(self, AlpmDB, &AlpmDB_type, ctx);
}

/* DB::update() : boolean
 * true if updated, false if up-to-date
 */
static VALUE update(int argc, VALUE *argv, VALUE self)
{
    AlpmDB *ctx;
    int res;
    VALUE force_rb;
    bool force;

    TypedData_Get_Struct(self, AlpmDB, &AlpmDB_type, ctx);

    /* check whether to force update; force only if an argument is given and is true */
    rb_scan_args(argc, argv, "01", &force_rb);
    if (RB_TYPE_P(force_rb, T_TRUE)) {
        force = true;
    } else {
        force = false;
    }
    
    res = alpm_db_update(force, ctx->db);

    if (res == 0) {
        return Qtrue;
    } else if (res == 1) {
        return Qfalse;
    } else {
        rb_raise(rb_eRuntimeError, "Failed to update database!");
        return Qnil;
    }
}

/* DB::name : String */
static VALUE db_name(VALUE self)
{
    AlpmDB *ctx;
    TypedData_Get_Struct(self, AlpmDB, &AlpmDB_type, ctx);

    return rb_str_new_cstr(alpm_db_get_name(ctx->db));
}

void Init_alpm_db(void)
{
    VALUE cDB = rb_define_class_under(mALPM, "DB", rb_cObject);

    rb_define_alloc_func(cDB, db_allocate);

    rb_define_method(cDB, "update", update, -1);
    rb_define_method(cDB, "name", db_name, 0);
}
