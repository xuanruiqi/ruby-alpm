#include <ruby.h>
#include <string.h>
#include <alpm.h>
#include <alpm_list.h>

#include "alpm_ruby.h"
#include "util.h"
#include "package.h"

static void package_free(void *ctx)
{
    alpm_pkg_t *pkg = ((AlpmPkg*) ctx)->pkg;
    int res;
    
    if (pkg) {
        res = alpm_pkg_free(pkg);
        if (res != 0) {
            rb_raise(rb_eRuntimeError, "Freeing package failed");
        }
    }
}

static size_t package_size(const void *ctx)
{
    (void) ctx;
    return sizeof(AlpmPkg);
}

static const rb_data_type_t AlpmPkg_type = {
    .wrap_struct_name = "ALPM::Package",
    .function = {
        .dmark = NULL,
        .dfree = package_free,
        .dsize = package_size,
    },
    .data = NULL,
    .flags = RUBY_TYPED_FREE_IMMEDIATELY,
};  

static VALUE package_allocate(VALUE self)
{
    AlpmPkg *ctx;
    return TypedData_Make_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);
}

/* Package::filename : String */
static VALUE filename(VALUE self)
{
    AlpmPkg *ctx;
    TypedData_Get_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);
    return rb_str_new_cstr(alpm_pkg_get_filename(ctx->pkg));
}

/* Package::base : String */
static VALUE base(VALUE self)
{
    AlpmPkg *ctx;
    TypedData_Get_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);
    return rb_str_new_cstr(alpm_pkg_get_base(ctx->pkg));
}

/* Package::version : String */
static VALUE version(VALUE self)
{
    AlpmPkg *ctx;
    TypedData_Get_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);
    return rb_str_new_cstr(alpm_pkg_get_version(ctx->pkg));
}

/* Package::desc : String */
static VALUE description(VALUE self)
{
    AlpmPkg *ctx;
    TypedData_Get_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);
    return rb_str_new_cstr(alpm_pkg_get_desc(ctx->pkg));
}

/* Package::url : String */
static VALUE url(VALUE self)
{
    AlpmPkg *ctx;
    TypedData_Get_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);
    return rb_str_new_cstr(alpm_pkg_get_url(ctx->pkg));
}

/* Package::build_date_ts : Number
 * Note: not intended to be used directly. Will need to be
 * converted to a DateTime.
 */
static VALUE build_date_ts(VALUE self)
{
    AlpmPkg *ctx;
    TypedData_Get_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);
    return INT2NUM(alpm_pkg_get_builddate(ctx->pkg));
}

/* Package::install_date_ts : Number
 * Note: not intended to be used directly. Will need to be
 * converted to a DateTime.
 */
static VALUE install_date_ts(VALUE self)
{
    AlpmPkg *ctx;
    TypedData_Get_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);
    return INT2NUM(alpm_pkg_get_installdate(ctx->pkg));
}

/* Package::packager : String */
static VALUE packager(VALUE self)
{
    AlpmPkg *ctx;
    TypedData_Get_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);
    return rb_str_new_cstr(alpm_pkg_get_packager(ctx->pkg));
}

/* Package::md5sum : String */
static VALUE md5sum(VALUE self)
{
    AlpmPkg *ctx;
    TypedData_Get_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);
    return rb_str_new_cstr(alpm_pkg_get_md5sum(ctx->pkg));
}

/* Package::sha256sum : String */
static VALUE sha256sum(VALUE self)
{
    AlpmPkg *ctx;
    TypedData_Get_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);
    return rb_str_new_cstr(alpm_pkg_get_sha256sum(ctx->pkg));
}

/* Package::size : Number */
static VALUE pkgsize(VALUE self)
{
    AlpmPkg *ctx;
    TypedData_Get_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);
    return INT2NUM(alpm_pkg_get_size(ctx->pkg));
}

/* Package::isize : Number */
static VALUE installed_size(VALUE self)
{
    AlpmPkg *ctx;
    TypedData_Get_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);
    return INT2NUM(alpm_pkg_get_isize(ctx->pkg));
}

/* Package::reason_raw : Number
 * Note: not intended to be used directly, will need to be converted
 * to a symbol
 */
static VALUE reason_raw(VALUE self)
{
    AlpmPkg *ctx;
    TypedData_Get_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);
    return INT2NUM(alpm_pkg_get_reason(ctx->pkg));
}

/* Package::validation_raw : Number
 * Note: not intended to be used directly, will need to be converted
 * to a symbol
 */
static VALUE validation_raw(VALUE self)
{
    AlpmPkg *ctx;
    TypedData_Get_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);
    return INT2NUM(alpm_pkg_get_validation(ctx->pkg));
}

/* Package::licenses : Array[String] */
static VALUE licenses(VALUE self)
{
    AlpmPkg *ctx;
    alpm_list_t *licenses;
    VALUE rb_licenses;
    
    TypedData_Get_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);
    
    licenses = alpm_pkg_get_licenses(ctx->pkg);
    rb_licenses = alpm_list_to_rb_ary(licenses, (VALUE (*)(void *)) rb_str_new_cstr);
    FREELIST(licenses);

    return rb_licenses;
}

/* Package::groups : Array[String] */
static VALUE groups(VALUE self)
{
    AlpmPkg *ctx;
    alpm_list_t *groups;
    VALUE rb_groups;
    
    TypedData_Get_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);
    
    groups = alpm_pkg_get_groups(ctx->pkg);
    rb_groups = alpm_list_to_rb_ary(groups, (VALUE (*)(void *)) rb_str_new_cstr);
    FREELIST(groups);

    return rb_groups;
}

/* Package::depends : Array[String] */
static VALUE depends(VALUE self)
{
    AlpmPkg *ctx;
    alpm_list_t *depends;
    VALUE rb_depends;
    
    TypedData_Get_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);
    
    depends = alpm_pkg_get_depends(ctx->pkg);
    rb_depends = alpm_list_to_rb_ary(depends, (VALUE (*)(void *)) rb_str_new_cstr);
    FREELIST(depends);

    return rb_depends;
}

/* Package::optdepends : Array[String] */
static VALUE optdepends(VALUE self)
{
    AlpmPkg *ctx;
    alpm_list_t *optdepends;
    VALUE rb_optdepends;
    
    TypedData_Get_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);
    
    optdepends = alpm_pkg_get_optdepends(ctx->pkg);
    rb_optdepends = alpm_list_to_rb_ary(optdepends, (VALUE (*)(void *)) rb_str_new_cstr);
    FREELIST(optdepends);

    return rb_optdepends;
}

/* Package::checkupdates : Array[String] */
static VALUE checkdepends(VALUE self)
{
    AlpmPkg *ctx;
    alpm_list_t *checkdepends;
    VALUE rb_checkdepends;
    
    TypedData_Get_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);
    
    checkdepends = alpm_pkg_get_checkdepends(ctx->pkg);
    rb_checkdepends = alpm_list_to_rb_ary(checkdepends, (VALUE (*)(void *)) rb_str_new_cstr);
    FREELIST(checkdepends);

    return rb_checkdepends;
}

/* Package::makedepends : Array[String] */
static VALUE makedepends(VALUE self)
{
    AlpmPkg *ctx;
    alpm_list_t *makedepends;
    VALUE rb_makedepends;
    
    TypedData_Get_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);
    
    makedepends = alpm_pkg_get_makedepends(ctx->pkg);
    rb_makedepends = alpm_list_to_rb_ary(makedepends, (VALUE (*)(void *)) rb_str_new_cstr);
    FREELIST(makedepends);

    return rb_makedepends;
}

/* Package::conflicts : Array[String] */
static VALUE conflicts(VALUE self)
{
    AlpmPkg *ctx;
    TypedData_Get_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);
    
    alpm_list_t *conflicts = alpm_pkg_get_conflicts(ctx->pkg);
    VALUE rb_conflicts = alpm_list_to_rb_ary(conflicts, (VALUE (*)(void *)) rb_str_new_cstr);
    FREELIST(conflicts);

    return rb_conflicts;
}

/* Package::provides : Array[String] */
static VALUE provides(VALUE self)
{
    AlpmPkg *ctx;
    alpm_list_t *provides;
    VALUE rb_provides;
    
    TypedData_Get_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);
    
    provides = alpm_pkg_get_provides(ctx->pkg);
     rb_provides = alpm_list_to_rb_ary(provides, (VALUE (*)(void *)) rb_str_new_cstr);
    FREELIST(provides);

    return rb_provides;
}

/* Package::replaces : Array[String] */
static VALUE replaces(VALUE self)
{
    AlpmPkg *ctx;
    alpm_list_t *replaces;
    VALUE rb_replaces;
    
    TypedData_Get_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);
    
    replaces = alpm_pkg_get_replaces(ctx->pkg);
    rb_replaces = alpm_list_to_rb_ary(replaces, (VALUE (*)(void *)) rb_str_new_cstr);
    FREELIST(replaces);

    return rb_replaces;
}

/* Package::files_raw */
static VALUE files_raw(VALUE self)
{
    AlpmPkg *ctx;
    TypedData_Get_Struct(self, AlpmPkg, &AlpmPkg_type, ctx);

    alpm_filelist_t* filelist = alpm_pkg_get_files(ctx->pkg);

    if (!filelist) {
        return Qnil;
    } else {
        size_t num_files = filelist->count;
        alpm_file_t *files = filelist->files;
        VALUE rb_files = rb_ary_new2((long) num_files);
        
        for (size_t i = 0; i < num_files; ++i) {
            alpm_file_t file_info = files[i];
            VALUE file_entry = rb_ary_new3(3, rb_str_new_cstr(file_info.name),
                INT2NUM(file_info.size), INT2NUM(file_info.mode));
            rb_ary_push(rb_files, file_entry);
        }
        
        return rb_files;
    } 
}

void init_alpm_package(void)
{
    VALUE cPackage = rb_define_class_under(mALPM, "Package", rb_cObject);

    rb_define_alloc_func(cPackage, package_allocate);

    rb_define_method(cPackage, "filename", filename, 0);
    rb_define_method(cPackage, "base", base, 0);
    rb_define_method(cPackage, "version", version, 0);
    rb_define_method(cPackage, "desc", description, 0);
    rb_define_method(cPackage, "url", url, 0);

    rb_define_private_method(cPackage, "build_date_ts", build_date_ts, 0);
    rb_define_private_method(cPackage, "install_date_ts", install_date_ts, 0);

    rb_define_method(cPackage, "packager", packager, 0);
    rb_define_method(cPackage, "md5sum", md5sum, 0);
    rb_define_method(cPackage, "sha256sum", sha256sum, 0);
    rb_define_method(cPackage, "size", pkgsize, 0);
    rb_define_method(cPackage, "isize", installed_size, 0);

    rb_define_private_method(cPackage, "reason_raw", reason_raw, 0);
    rb_define_private_method(cPackage, "validation_raw", validation_raw, 0);

    rb_define_method(cPackage, "licenses", licenses, 0);
    rb_define_method(cPackage, "groups", groups, 0);
    rb_define_method(cPackage, "depends", depends, 0);
    rb_define_method(cPackage, "optdepends", optdepends, 0);
    rb_define_method(cPackage, "checkdepends", checkdepends, 0);
    rb_define_method(cPackage, "makedepends", makedepends, 0);
    rb_define_method(cPackage, "conflicts", conflicts, 0);
    rb_define_method(cPackage, "provides", provides, 0);
    rb_define_method(cPackage, "replaces", replaces, 0);

    rb_define_method(cPackage, "files_raw", files_raw, 0);
}

