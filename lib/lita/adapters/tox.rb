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
      def initialize(robot)
        super

        @tox = ::Tox.new

        log.info("ID: #{@tox.id}")

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

      def send_messages(target, messages)
        messages.reject(&:empty?).each do |message|
          @tox.friend_send_message(target.user.id.to_i, message)
        end
      end
    end

    Lita.register_adapter(:tox, Tox)
  end
end
