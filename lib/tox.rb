require 'tox/tox'

class Tox
  attr_accessor :running

  def initialize(options = Tox::Options.new)
    initialize_with(options)
  end
end
