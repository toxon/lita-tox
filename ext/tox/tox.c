#include <ruby.h>

#include <time.h>

#include <tox/tox.h>

#define TOX_IS_COMPATIBLE TOX_VERSION_IS_API_COMPATIBLE
TOX_VERSION_REQUIRE(0, 0, 0);

void Init_tox();

#define IP_LENGTH_MAX 15

typedef char IP[IP_LENGTH_MAX + 1];
typedef char Key[(TOX_PUBLIC_KEY_SIZE * 2) + 1];

typedef struct Node {
  IP ip;
  int port;
  Key key;
} Node;

typedef uint8_t KeyBin[TOX_PUBLIC_KEY_SIZE];

static void Key_to_KeyBin(const char *key, uint8_t *key_bin);

typedef struct cTox_ {
  Tox *tox;
} cTox_;

static VALUE cTox;

static VALUE cTox_alloc(VALUE klass);
static void  cTox_free(void *ptr);
static VALUE cTox_initialize_with(VALUE self, VALUE options);
static VALUE cTox_savedata(VALUE self);
static VALUE cTox_id(VALUE self);
static VALUE cTox_bootstrap(VALUE self, VALUE options);
static VALUE cTox_kill(VALUE self);
static VALUE cTox_loop(VALUE self);
static VALUE cTox_friend_add_norequest(VALUE self, VALUE key);
static VALUE cTox_friend_send_message(VALUE self, VALUE friend_number, VALUE text);
static VALUE cTox_join_groupchat(VALUE self, VALUE friend_number, VALUE data);
static VALUE cTox_group_message_send(VALUE self, VALUE group_number, VALUE text);

static void on_friend_request(
  Tox *tox,
  const uint8_t *key,
  const uint8_t *data,
  size_t length,
  VALUE self);

static void on_friend_message(
  Tox *tox,
  uint32_t friend_number,
  TOX_MESSAGE_TYPE type,
  const uint8_t *text,
  size_t length,
  VALUE self);

typedef struct Tox_Options cTox_cOptions_;

static VALUE cTox_cOptions;

static VALUE cTox_cOptions_alloc(VALUE klass);
static void  cTox_cOptions_free(void *ptr);
static VALUE cTox_cOptions_initialize(VALUE self);
static VALUE cTox_cOptions_data_EQUALS(VALUE self, VALUE data);

void Init_tox()
{
  if (!TOX_VERSION_IS_ABI_COMPATIBLE())
    rb_raise(rb_eLoadError, "incompatible Tox ABI version");

  cTox = rb_define_class("Tox", rb_cObject);
  rb_define_alloc_func(cTox, cTox_alloc);
  rb_define_method(cTox, "initialize_with", cTox_initialize_with, 1);
  rb_define_method(cTox, "savedata", cTox_savedata, 0);
  rb_define_method(cTox, "id", cTox_id, 0);
  rb_define_method(cTox, "bootstrap", cTox_bootstrap, 1);
  rb_define_method(cTox, "kill", cTox_kill, 0);
  rb_define_method(cTox, "loop", cTox_loop, 0);
  rb_define_method(cTox, "friend_add_norequest", cTox_friend_add_norequest, 1);
  rb_define_method(cTox, "friend_send_message", cTox_friend_send_message, 2);
  rb_define_method(cTox, "join_groupchat", cTox_join_groupchat, 2);
  rb_define_method(cTox, "group_message_send", cTox_group_message_send, 2);

  cTox_cOptions = rb_define_class_under(cTox, "Options", rb_cObject);
  rb_define_alloc_func(cTox_cOptions, cTox_cOptions_alloc);
  rb_define_method(cTox_cOptions, "initialize", cTox_cOptions_initialize, 0);
  rb_define_method(cTox_cOptions, "data=", cTox_cOptions_data_EQUALS, 1);
}

/******************************************************************************
 * KeyBin
 ******************************************************************************/

void Key_to_KeyBin(const char *const key, uint8_t *const key_bin)
{
  long i;

  for (i = 0; i < TOX_PUBLIC_KEY_SIZE; ++i)
    sscanf(&key[i * 2], "%2hhx", &key_bin[i]);
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

VALUE cTox_initialize_with(const VALUE self, const VALUE options)
{
  cTox_ *tox;
  cTox_cOptions_ *tox_options;

  TOX_ERR_NEW error;

  // check if `options` is instance of `Tox::Options`
  if (Qfalse == rb_funcall(options, rb_intern("is_a?"), 1, cTox_cOptions))
    rb_raise(rb_eTypeError, "argument 1 should be Tox::Options");

  Data_Get_Struct(self, cTox_, tox);
  Data_Get_Struct(options, cTox_cOptions_, tox_options);

  tox->tox = tox_new(tox_options, &error);

  if (error != TOX_ERR_NEW_OK)
    rb_raise(rb_eRuntimeError, "tox_new() failed");

  tox_callback_friend_request(tox->tox, (tox_friend_request_cb*)on_friend_request, (void*)self);
  tox_callback_friend_message(tox->tox, (tox_friend_message_cb*)on_friend_message, (void*)self);

  return self;
}

VALUE cTox_savedata(const VALUE self)
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

VALUE cTox_id(const VALUE self)
{
  cTox_ *tox;

  char address[TOX_ADDRESS_SIZE];
  char id[2 * TOX_ADDRESS_SIZE];

  unsigned long i;

  Data_Get_Struct(self, cTox_, tox);

  tox_self_get_address(tox->tox, (uint8_t*)address);

  for (i = 0; i < TOX_ADDRESS_SIZE; ++i)
    sprintf(&id[2 * i], "%02X", address[i] & 0xFF);

  return rb_str_new(id, 2 * TOX_ADDRESS_SIZE);
}

VALUE cTox_bootstrap(const VALUE self, const VALUE options)
{
  cTox_ *tox;

  VALUE ip;
  VALUE port;
  VALUE key;

  Node node;
  KeyBin key_bin;

  TOX_ERR_BOOTSTRAP error;

  Check_Type(options, T_HASH);

  ip   = rb_hash_aref(options, ID2SYM(rb_intern("ip")));
  port = rb_hash_aref(options, ID2SYM(rb_intern("port")));
  key  = rb_hash_aref(options, ID2SYM(rb_intern("key")));

  if (ip == Qnil)   rb_raise(rb_eRuntimeError, "option \"ip\" is required");
  if (port == Qnil) rb_raise(rb_eRuntimeError, "option \"port\" is required");
  if (key == Qnil)  rb_raise(rb_eRuntimeError, "option \"key\" is required");

  Data_Get_Struct(self, cTox_, tox);

  memcpy(node.ip, RSTRING_PTR(ip), RSTRING_LEN(ip) + 1);
  node.port = NUM2INT(port);
  memcpy(node.key, RSTRING_PTR(key), RSTRING_LEN(key) + 1);

  Key_to_KeyBin(node.key, key_bin);

  tox_bootstrap(tox->tox, node.ip, node.port, key_bin, &error);

  if (error == TOX_ERR_BOOTSTRAP_OK)
    return Qtrue;
  else
    return Qfalse;
}

VALUE cTox_kill(const VALUE self)
{
  cTox_ *tox;

  Data_Get_Struct(self, cTox_, tox);

  tox_kill(tox->tox);

  return self;
}

VALUE cTox_loop(const VALUE self)
{
  cTox_ *tox;

  struct timespec delay;

  Data_Get_Struct(self, cTox_, tox);

  rb_funcall(self, rb_intern("running="), 1, Qtrue);

  delay.tv_sec = 0;

  while (rb_funcall(self, rb_intern("running"), 0)) {
    delay.tv_nsec = tox_iteration_interval(tox->tox) * 1000000;
    nanosleep(&delay, NULL);

    tox_iterate(tox->tox);
  }

  return self;
}

VALUE cTox_friend_add_norequest(const VALUE self, const VALUE key)
{
  cTox_ *tox;

  Check_Type(key, T_STRING);

  Data_Get_Struct(self, cTox_, tox);

  return LONG2FIX(tox_friend_add_norequest(tox->tox, (uint8_t*)RSTRING_PTR(key), NULL));
}

VALUE cTox_friend_send_message(const VALUE self, const VALUE friend_number, const VALUE text)
{
  cTox_ *tox;

  // Don't know yet how to check for integers
  // Check_Type(friend_number, T_INTEGER);
  Check_Type(text, T_STRING);

  Data_Get_Struct(self, cTox_, tox);

  return LONG2FIX(tox_friend_send_message(
    tox->tox,
    NUM2LONG(friend_number),
    TOX_MESSAGE_TYPE_NORMAL,
    (uint8_t*)RSTRING_PTR(text),
    RSTRING_LEN(text),
    NULL
  ));
}

VALUE cTox_join_groupchat(const VALUE self, const VALUE friend_number, const VALUE data)
{
  cTox_ *tox;

  // Don't know yet how to check for integers
  // Check_Type(friend_number, T_INTEGER);
  Check_Type(data, T_STRING);

  Data_Get_Struct(self, cTox_, tox);

  return INT2FIX(tox_join_groupchat(
    tox->tox,
    NUM2LONG(friend_number),
    (uint8_t*)RSTRING_PTR(data),
    RSTRING_LEN(data)
  ));
}

VALUE cTox_group_message_send(const VALUE self, const VALUE group_number, const VALUE text)
{
  cTox_ *tox;

  // Don't know yet how to check for integers
  // Check_Type(group_number, T_INTEGER);
  Check_Type(text, T_STRING);

  Data_Get_Struct(self, cTox_, tox);

  return INT2FIX(tox_group_message_send(
    tox->tox,
    NUM2LONG(group_number),
    (uint8_t*)RSTRING_PTR(text),
    RSTRING_LEN(text)
  ));
}

void on_friend_request(
  Tox *const tox,
  const uint8_t *const key,
  const uint8_t *const data,
  const size_t length,
  const VALUE self)
{
  VALUE rb_on_friend_request;

  rb_on_friend_request = rb_iv_get(self, "@on_friend_request");

  if (Qnil != rb_on_friend_request)
    rb_funcall(
      rb_on_friend_request,
      rb_intern("call"),
      2,
      rb_str_new((char*)key, TOX_PUBLIC_KEY_SIZE),
      rb_str_new((char*)data, length)
    );
}

void on_friend_message(
  Tox *const tox,
  const uint32_t friend_number,
  const TOX_MESSAGE_TYPE type,
  const uint8_t *const text,
  const size_t length,
  const VALUE self)
{
  VALUE rb_on_friend_message;

  if (type != TOX_MESSAGE_TYPE_NORMAL)
    return;

  rb_on_friend_message = rb_iv_get(self, "@on_friend_message");

  if (Qnil != rb_on_friend_message)
    rb_funcall(
      rb_on_friend_message,
      rb_intern("call"),
      2,
      LONG2FIX(friend_number),
      rb_str_new((char*)text, length)
    );
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

  tox_options_default(tox_options);

  return self;
}

VALUE cTox_cOptions_data_EQUALS(const VALUE self, const VALUE savedata)
{
  cTox_cOptions_ *tox_options;

  Check_Type(savedata, T_STRING);

  Data_Get_Struct(self, cTox_cOptions_, tox_options);

  tox_options->savedata_type = TOX_SAVEDATA_TYPE_TOX_SAVE;
  tox_options->savedata_data = (uint8_t*)RSTRING_PTR(savedata);
  tox_options->savedata_length = RSTRING_LEN(savedata);

  return savedata;
}
