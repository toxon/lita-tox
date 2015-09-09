#include <ruby.h>

#include <tox/tox.h>

void Init_tox();

static VALUE cTox;

static VALUE cTox_cOptions;
static VALUE cTox_cOptions_alloc(VALUE klass);
static void  cTox_cOptions_free(void *ptr);

void Init_tox()
{
  cTox = rb_define_class("Tox", rb_cObject);

  cTox_cOptions = rb_define_class_under(cTox, "Options", rb_cObject);
  rb_define_alloc_func(cTox_cOptions, cTox_cOptions_alloc);
}

VALUE cTox_cOptions_alloc(const VALUE klass)
{
  struct Tox_Options *tox_options;

  tox_options = ALLOC(struct Tox_Options);

  return Data_Wrap_Struct(klass, NULL, cTox_cOptions_free, tox_options);
}

void cTox_cOptions_free(void *const ptr)
{
  free(ptr);
}
