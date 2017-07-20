# coding: utf-8

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

Gem::Specification.new do |spec|
  spec.name          = 'lita-tox'
  spec.version       = '0.3.0'
  spec.authors       = ['Braiden Vasco']
  spec.email         = ['braiden-vasco@mailtor.net']

  spec.summary       = 'Tox adapter for the Lita chat bot'
  spec.description   = 'Tox adapter for the Lita chat bot.'
  spec.homepage      = 'https://github.com/braiden-vasco/lita-tox'
  spec.license       = 'GPL-3.0'

  spec.metadata['lita_plugin_type'] = 'adapter'

  spec.files = `git ls-files -z`.split("\x0").reject do |f|
    f.match(%r{^(test|spec|features)/})
  end

  spec.bindir        = 'exe'
  spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.require_paths = ['lib']

  spec.extensions << 'ext/tox/extconf.rb'

  spec.add_development_dependency 'bundler', '~> 1.7'
  spec.add_development_dependency 'rake', '~> 10.0'
  spec.add_development_dependency 'rubocop', '~> 0.31'
  spec.add_development_dependency 'rspec', '~> 3.3'
  spec.add_development_dependency 'simplecov', '~> 0.10'
  spec.add_development_dependency 'yard', '~> 0.8'
  spec.add_development_dependency 'redcarpet', '~> 3.3'
  spec.add_development_dependency 'github_changelog_generator', '~> 1.6'
  spec.add_development_dependency 'rake-compiler', '~> 0.9'
  spec.add_development_dependency 'pry', '~> 0.10'

  spec.add_runtime_dependency 'lita', '~> 4.4'
end
