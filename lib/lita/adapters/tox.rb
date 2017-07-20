# lita-tox - Tox adapter for the Lita chat bot
# Copyright (C) 2015-2017  Braiden Vasco
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

require 'tox'

##
# Lita module.
#
module Lita
  ##
  # Lita adapters module.
  #
  module Adapters
    ##
    # Tox adapter for the Lita chat bot.
    #
    class Tox < Adapter
      config :savedata_filename, type: String
      config :status, type: String

      def initialize(robot)
        super

        options = ::Tox::Options.new

        if config.savedata_filename && File.exist?(config.savedata_filename)
          savedata_file = open(config.savedata_filename)
          options.data = savedata_file.read
          savedata_file.close
        end

        @tox = ::Tox.new(options)

        log.info("ID: #{@tox.id}")

        @tox.name = robot.name if robot.name
        @tox.status_message = config.status if config.status

        @tox.on_friend_request do |key|
          @tox.friend_add_norequest(key)
        end

        @tox.on_friend_message do |friend_number, text|
          user = User.new(friend_number)
          source = Source.new(user: user)
          message = Message.new(robot, text, source)

          message.command!
          robot.receive(message)
        end
      end

      def run
        @tox.loop
      end

      def shut_down
        if config.savedata_filename
          savedata_file = open(config.savedata_filename, 'w')
          savedata_file.write(@tox.savedata)
          savedata_file.close
        end

        @tox.kill
      end

      def send_messages(target, messages)
        messages.reject(&:empty?).each do |message|
          @tox.friend_send_message(target.user.id.to_i, message)
        end
      end
    end

    Lita.register_adapter(:tox, Tox)
  end
end
