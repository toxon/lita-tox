Lita::Adapters::Tox
===================

[![Gem Version](https://badge.fury.io/rb/lita-tox.svg)](http://badge.fury.io/rb/lita-tox)
[![Build Status](https://travis-ci.org/toxon/lita-tox.svg)](https://travis-ci.org/toxon/lita-tox)
[![Coverage Status](https://coveralls.io/repos/github/toxon/lita-tox/badge.svg)](https://coveralls.io/github/toxon/lita-tox)

[Tox](https://tox.chat) adapter for the [Lita](http://lita.io) chat bot.


Usage
-----

At first, see the documentation for Lita: http://docs.lita.io/

### Installation

**libtoxcore** should be compiled manually at your computer or server.
Follow the instructions in
[that file](https://github.com/irungentoo/toxcore/blob/2ab3b14731061cc04d3ccc50a35093c11d018298/INSTALL.md)

When **libtoxcore** is installed, add **lita-tox**
to your Lita instance's Gemfile:

```ruby
gem 'lita-tox', '~> 0.5.0'
```

### Configuration

`config.robot.name` will be used as Tox user name

Mentions in Tox usually use user name, Tox clients usually allow mentioning
by writing first letters of user name and pressing `<Tab>`, so don't use
`config.robot.mention_name`

#### Optional attributes

- `savedata_filename` (String) - Path to file where Tox state will be stored (if provided)
- `status` (String) - Tox user status

#### Example

This is an example `lita_config.rb` file:

```ruby
Lita.configure do |config|
  config.robot.name = 'Lita chat bot'

  config.robot.adapter = :tox

  config.adapters.tox.savedata_filename = 'savedata'
  config.adapters.tox.status = "Send me \"#{config.robot.name}: help\""
end
```
