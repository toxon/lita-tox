#!/usr/bin/env ruby

require 'mkmf'

NAME = 'tox'

LIBTOXCORE = 'toxcore'

have_library LIBTOXCORE, 'tox_options_default' and
have_library LIBTOXCORE, 'tox_new' and
have_library LIBTOXCORE, 'tox_get_savedata_size' and
have_library LIBTOXCORE, 'tox_get_savedata' and
have_library LIBTOXCORE, 'tox_self_get_address' and
have_library LIBTOXCORE, 'tox_bootstrap' and
have_library LIBTOXCORE, 'tox_kill' and
have_library LIBTOXCORE, 'tox_version_is_compatible' and

create_makefile "#{NAME}/#{NAME}" or exit(1)
