#include <ruby.h>

#include <tox/tox.h>

void Init_tox();

typedef struct cTox_ {
  Tox *tox;
} cTox_;

static VALUE cTox;
static VALUE cTox_alloc(VALUE klass);
static void  cTox_free(void *ptr);
static VALUE cTox_initialize(VALUE self, VALUE options);
static VALUE cTox_savedata(VALUE self);

typedef struct Tox_Options cTox_cOptions_;

static VALUE cTox_cOptions;
static VALUE cTox_cOptions_alloc(VALUE klass);
static void  cTox_cOptions_free(void *ptr);
static VALUE cTox_cOptions_initialize(VALUE self);
static VALUE cTox_cOptions_data_EQUALS(VALUE self, VALUE data);

void Init_tox()
{
  cTox = rb_define_class("Tox", rb_cObject);
  rb_define_alloc_func(cTox, cTox_alloc);
  rb_define_method(cTox, "initialize", cTox_initialize, 1);
  rb_define_method(cTox, "savedata", cTox_savedata, 0);

  cTox_cOptions = rb_define_class_under(cTox, "Options", rb_cObject);
  rb_define_alloc_func(cTox_cOptions, cTox_cOptions_alloc);
  rb_define_method(cTox_cOptions, "initialize", cTox_cOptions_initialize, 0);
  rb_define_method(cTox_cOptions, "data=", cTox_cOptions_data_EQUALS, 1);
}

/******************************************************************************
 * Tox
 ******************************************************************************/

VALUE cTox_alloc(const VALUE klass)
{
  cTox_ *tox;

  tox = ALLOC(cTox_);

  return Data_Wrap_Struct(klass, NULL, cTox_free, tox);
}

void cTox_free(void *const ptr)
{
  free(ptr);
}

VALUE cTox_initialize(const VALUE self, const VALUE options)
{
  cTox_ *tox;
  cTox_cOptions_ *tox_options;

  // check if `options` is instance of `Tox::Options`
  if (Qfalse == rb_funcall(options, rb_intern("is_a?"), 1, cTox_cOptions))
    rb_raise(rb_eTypeError, "argument 1 should be Tox::Options");

  Data_Get_Struct(self, cTox_, tox);
  Data_Get_Struct(options, cTox_cOptions_, tox_options);

  tox->tox = tox_new(tox_options, NULL);

  return self;
}

VALUE cTox_savedata(VALUE self)
{
  cTox_ *tox;

  size_t data_size;
  char *data;

  Data_Get_Struct(self, cTox_, tox);

  data_size = tox_get_savedata_size(tox->tox);
  data = ALLOC_N(char, data_size);

  tox_get_savedata(tox->tox, (uint8_t*)data);

  return rb_str_new(data, data_size);
}

/******************************************************************************
 * Tox::Options
 ******************************************************************************/

VALUE cTox_cOptions_alloc(const VALUE klass)
{
  cTox_cOptions_ *tox_options;

  tox_options = ALLOC(cTox_cOptions_);

  return Data_Wrap_Struct(klass, NULL, cTox_cOptions_free, tox_options);
}

void cTox_cOptions_free(void *const ptr)
{
  free(ptr);
}

VALUE cTox_cOptions_initialize(const VALUE self)
{
  cTox_cOptions_ *tox_options;

  Data_Get_Struct(self, cTox_cOptions_, tox_options);

  memset(tox_options, 0, sizeof(cTox_cOptions_));
  tox_options_default(tox_options);

  return self;
}

VALUE cTox_cOptions_data_EQUALS(VALUE self, VALUE savedata)
{
  cTox_cOptions_ *tox_options;

  Check_Type(savedata, T_STRING);

  Data_Get_Struct(self, cTox_cOptions_, tox_options);

  tox_options->savedata_type = TOX_SAVEDATA_TYPE_TOX_SAVE;
  tox_options->savedata_data = (uint8_t*)RSTRING_PTR(savedata);
  tox_options->savedata_length = RSTRING_LEN(savedata);

  return savedata;
}
