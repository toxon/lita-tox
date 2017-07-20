# frozen_string_literal: true

Lita.configure do |config|
  config.robot.name = 'Lita'

  config.robot.adapter = :tox

  config.adapters.tox.savedata_filename = 'savedata'
  config.adapters.tox.status = "Send me \"#{config.robot.name}: help\""
end
