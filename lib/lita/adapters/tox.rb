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

        @tox.on_group_invite do |friend_number, data|
          @tox.join_groupchat(friend_number, data)
        end

        @tox.on_group_message do |group_number, peer_number, text|
          unless @tox.group_peernumber_is_ours(group_number, peer_number)
            user = User.new(-1 - peer_number) # TODO
            source = Source.new(user: user, room: group_number)
            message = Message.new(robot, text, source)

            robot.receive(message)
          end
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
          if target.user.id.to_i >= 0
            @tox.friend_send_message(target.user.id.to_i, message)
          else
            @tox.group_message_send(target.room.to_i, message)
          end
        end
      end
    end

    Lita.register_adapter(:tox, Tox)
  end
end
