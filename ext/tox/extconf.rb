#!/usr/bin/env ruby

require 'mkmf'

NAME = 'tox'

LIBTOXCORE = 'toxcore'
have_library LIBTOXCORE, 'tox_options_default'
have_library LIBTOXCORE, 'tox_new'
have_library LIBTOXCORE, 'tox_get_savedata_size'
have_library LIBTOXCORE, 'tox_get_savedata'
have_library LIBTOXCORE, 'tox_self_get_address'

create_makefile "#{NAME}/#{NAME}"
