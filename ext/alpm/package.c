#include <ruby.h>
#include <string.h>
#include <alpm.h>
#include <alpm_list.h>

#include "alpm_ruby.h"
#include "util.h"
#include "dependency.h"
#include "package.h"

static void package_free(void *ctx)
{
    alpm_pkg_t *pkg = *((alpm_pkg_t**) ctx);
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
    return sizeof(*((alpm_pkg_t**) ctx));
}

const rb_data_type_t AlpmPkg_type = {
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
    alpm_pkg_t **ctx;
    return TypedData_Make_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);
}

/* Package::filename : String */
static VALUE filename(VALUE self)
{
    alpm_pkg_t **ctx;
    TypedData_Get_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);
    return rb_str_new_cstr(alpm_pkg_get_filename(*ctx));
}

/* Package::base : String */
static VALUE base(VALUE self)
{
    alpm_pkg_t **ctx;
    TypedData_Get_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);
    return rb_str_new_cstr(alpm_pkg_get_base(*ctx));
}

/* Package::version : String */
static VALUE version(VALUE self)
{
    alpm_pkg_t **ctx;
    TypedData_Get_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);
    return rb_str_new_cstr(alpm_pkg_get_version(*ctx));
}

/* Package::desc : String */
static VALUE description(VALUE self)
{
    alpm_pkg_t **ctx;
    TypedData_Get_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);
    return rb_str_new_cstr(alpm_pkg_get_desc(*ctx));
}

/* Package::url : String */
static VALUE url(VALUE self)
{
    alpm_pkg_t **ctx;
    TypedData_Get_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);
    return rb_str_new_cstr(alpm_pkg_get_url(*ctx));
}

/* Package::build_date_ts : Number
 * Note: not intended to be used directly. Will need to be
 * converted to a DateTime.
 */
static VALUE build_date_ts(VALUE self)
{
    alpm_pkg_t **ctx;
    TypedData_Get_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);
    return INT2NUM(alpm_pkg_get_builddate(*ctx));
}

/* Package::install_date_ts : Number
 * Note: not intended to be used directly. Will need to be
 * converted to a DateTime.
 */
static VALUE install_date_ts(VALUE self)
{
    alpm_pkg_t **ctx;
    TypedData_Get_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);
    return INT2NUM(alpm_pkg_get_installdate(*ctx));
}

/* Package::packager : String */
static VALUE packager(VALUE self)
{
    alpm_pkg_t **ctx;
    TypedData_Get_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);
    return rb_str_new_cstr(alpm_pkg_get_packager(*ctx));
}

/* Package::md5sum : String */
static VALUE md5sum(VALUE self)
{
    alpm_pkg_t **ctx;
    TypedData_Get_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);
    return rb_str_new_cstr(alpm_pkg_get_md5sum(*ctx));
}

/* Package::sha256sum : String */
static VALUE sha256sum(VALUE self)
{
    alpm_pkg_t **ctx;
    TypedData_Get_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);
    return rb_str_new_cstr(alpm_pkg_get_sha256sum(*ctx));
}

/* Package::size : Number */
static VALUE pkgsize(VALUE self)
{
    alpm_pkg_t **ctx;
    TypedData_Get_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);
    return INT2NUM(alpm_pkg_get_size(*ctx));
}

/* Package::isize : Number */
static VALUE installed_size(VALUE self)
{
    alpm_pkg_t **ctx;
    TypedData_Get_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);
    return INT2NUM(alpm_pkg_get_isize(*ctx));
}

/* Package::reason_raw : Number
 * Note: not intended to be used directly, will need to be converted
 * to a symbol
 */
static VALUE reason_raw(VALUE self)
{
    alpm_pkg_t **ctx;
    TypedData_Get_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);
    return INT2NUM(alpm_pkg_get_reason(*ctx));
}

/* Package::validation_raw : Number
 * Note: not intended to be used directly, will need to be converted
 * to a symbol
 */
static VALUE validation_raw(VALUE self)
{
    alpm_pkg_t **ctx;
    TypedData_Get_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);
    return INT2NUM(alpm_pkg_get_validation(*ctx));
}

/* Package::licenses : Array[String] */
static VALUE licenses(VALUE self)
{
    alpm_pkg_t **ctx;
    alpm_list_t *licenses;
    VALUE rb_licenses;
    
    TypedData_Get_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);
    
    licenses = alpm_pkg_get_licenses(*ctx);
    rb_licenses = alpm_list_to_rb_ary(licenses, (VALUE (*)(void *)) rb_str_new_cstr);
    FREELIST(licenses);

    return rb_licenses;
}

/* Package::groups : Array[String] */
static VALUE groups(VALUE self)
{
    alpm_pkg_t **ctx;
    alpm_list_t *groups;
    VALUE rb_groups;
    
    TypedData_Get_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);
    
    groups = alpm_pkg_get_groups(*ctx);
    rb_groups = alpm_list_to_rb_ary(groups, (VALUE (*)(void *)) dependency_make);
    FREELIST(groups);

    return rb_groups;
}

/* Package.depends : Array[Dependency] */
static VALUE depends(VALUE self)
{
    alpm_pkg_t **ctx;
    alpm_list_t *depends;
    VALUE rb_depends;
    
    TypedData_Get_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);
    
    depends = alpm_pkg_get_depends(*ctx);
    rb_depends = alpm_list_to_rb_ary(depends, (VALUE (*)(void *)) dependency_make);
    FREELIST(depends);

    return rb_depends;
}

/* Package.optdepends : Array[Dependency] */
static VALUE optdepends(VALUE self)
{
    alpm_pkg_t **ctx;
    alpm_list_t *optdepends;
    VALUE rb_optdepends;
    
    TypedData_Get_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);
    
    optdepends = alpm_pkg_get_optdepends(*ctx);
    rb_optdepends = alpm_list_to_rb_ary(optdepends, (VALUE (*)(void *)) dependency_make);
    FREELIST(optdepends);

    return rb_optdepends;
}

/* Package.checkupdates : Array[Dependency] */
static VALUE checkdepends(VALUE self)
{
    alpm_pkg_t **ctx;
    alpm_list_t *checkdepends;
    VALUE rb_checkdepends;
    
    TypedData_Get_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);
    
    checkdepends = alpm_pkg_get_checkdepends(*ctx);
    rb_checkdepends = alpm_list_to_rb_ary(checkdepends, (VALUE (*)(void *)) dependency_make);
    FREELIST(checkdepends);

    return rb_checkdepends;
}

/* Package.makedepends : Array[Dependency] */
static VALUE makedepends(VALUE self)
{
    alpm_pkg_t **ctx;
    alpm_list_t *makedepends;
    VALUE rb_makedepends;
    
    TypedData_Get_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);
    
    makedepends = alpm_pkg_get_makedepends(*ctx);
    rb_makedepends = alpm_list_to_rb_ary(makedepends, (VALUE (*)(void *)) dependency_make);
    FREELIST(makedepends);

    return rb_makedepends;
}

/* Package.conflicts : Array[Dependency] */
static VALUE conflicts(VALUE self)
{
    alpm_pkg_t **ctx;
    TypedData_Get_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);
    
    alpm_list_t *conflicts = alpm_pkg_get_conflicts(*ctx);
    VALUE rb_conflicts = alpm_list_to_rb_ary(conflicts, (VALUE (*)(void *)) dependency_make);
    FREELIST(conflicts);

    return rb_conflicts;
}

/* Package.provides : Array[Dependency] */
static VALUE provides(VALUE self)
{
    alpm_pkg_t **ctx;
    alpm_list_t *provides;
    VALUE rb_provides;
    
    TypedData_Get_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);
    
    provides = alpm_pkg_get_provides(*ctx);
     rb_provides = alpm_list_to_rb_ary(provides, (VALUE (*)(void *)) dependency_make);
    FREELIST(provides);

    return rb_provides;
}

/* Package.replaces : Array[Dependency] */
static VALUE replaces(VALUE self)
{
    alpm_pkg_t **ctx;
    alpm_list_t *replaces;
    VALUE rb_replaces;
    
    TypedData_Get_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);
    
    replaces = alpm_pkg_get_replaces(*ctx);
    rb_replaces = alpm_list_to_rb_ary(replaces, (VALUE (*)(void *)) dependency_make);
    FREELIST(replaces);

    return rb_replaces;
}

/* Package.files_raw: Array[[String, Num, Num]] */
static VALUE files_raw(VALUE self)
{
    alpm_pkg_t **ctx;
    TypedData_Get_Struct(self, alpm_pkg_t*, &AlpmPkg_type, ctx);

    alpm_filelist_t* filelist = alpm_pkg_get_files(*ctx);

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

VALUE cPackage;

void init_alpm_package(void)
{
    cPackage = rb_define_class_under(mALPM, "Package", rb_cObject);

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

