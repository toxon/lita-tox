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
have_library LIBTOXCORE, 'tox_iteration_interval' and
have_library LIBTOXCORE, 'tox_iterate' and
have_library LIBTOXCORE, 'tox_friend_add_norequest' and
have_library LIBTOXCORE, 'tox_friend_send_message' and
have_library LIBTOXCORE, 'tox_callback_friend_request' and
have_library LIBTOXCORE, 'tox_callback_friend_message' and

create_makefile "#{NAME}/#{NAME}" or exit(1)
