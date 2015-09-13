Lita::Adapters::Tox
===================

[![Gem Version](https://badge.fury.io/rb/lita-tox.svg)](http://badge.fury.io/rb/lita-tox)
[![Build Status](https://travis-ci.org/braiden-vasco/lita-tox.svg)](https://travis-ci.org/braiden-vasco/lita-tox)
[![Coverage Status](https://coveralls.io/repos/braiden-vasco/lita-tox/badge.svg)](https://coveralls.io/r/braiden-vasco/lita-tox)

[Tox](https://tox.chat) adapter for the [Lita](http://lita.io) chat bot.

TODO
----

Current development version have some limitations
which should be fixed in first release:

* Adapter doesn't save Tox state, so you have to send friendship
  request again after each run. Current Tox ID is shown at start
  as Lita's information message

* Only private chats are supported. Adapter will not respond to group invite

* **libtoxcore** is not included in the gem. It should be compiled manually
  to build the gem native extension successfully (see the instructions below)

* JRuby is not supported. Only C extension for Tox is implemented

* Message length is limited to value of `TOX_MAX_MESSAGE_LENGTH`
  (see [source code of **libtoxcore**](https://github.com/irungentoo/toxcore/blob/2ab3b14731061cc04d3ccc50a35093c11d018298/toxcore/tox.h#L252-L255))

* Adapter has incomplete API for Lita. Only basic methods are implemented
