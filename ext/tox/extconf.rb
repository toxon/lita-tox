#!/usr/bin/env ruby

require 'mkmf'

NAME = 'tox'

LIBTOXCORE = 'toxcore'
have_library LIBTOXCORE, 'tox_options_default'

create_makefile "#{NAME}/#{NAME}"
