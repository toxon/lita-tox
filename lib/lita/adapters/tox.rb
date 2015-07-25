module Lita
  module Adapters
    class Tox < Adapter
    end

    Lita.register_adapter(:tox, Tox)
  end
end
