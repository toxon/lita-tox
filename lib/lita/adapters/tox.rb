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
    end

    Lita.register_adapter(:tox, Tox)
  end
end
