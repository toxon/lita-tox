require 'tox/tox'

class Tox
  attr_accessor :running

  def initialize(options = Tox::Options.new)
    initialize_with(options)
  end

  def on_friend_request(&block)
    @on_friend_request = block
  end

  def on_friend_message(&block)
    @on_friend_message = block
  end
end
