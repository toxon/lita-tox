# coding: utf-8
# frozen_string_literal: true

lib = File.expand_path('lib', __dir__).freeze
$LOAD_PATH.unshift lib unless $LOAD_PATH.include? lib

Gem::Specification.new do |spec|
  spec.name     = 'lita-tox'
  spec.version  = '0.5.0'
  spec.license  = 'GPL-3.0'
  spec.homepage = 'https://github.com/toxon/lita-tox'
  spec.summary  = 'Tox adapter for the Lita chat bot'
  spec.platform = Gem::Platform::RUBY

  spec.authors = ['Braiden Vasco']
  spec.email   = %w[braiden-vasco@users.noreply.github.com]

  spec.required_ruby_version = '~> 2.3'

  spec.description = 'Tox adapter for the Lita chat bot.'

  spec.metadata['lita_plugin_type'] = 'adapter'

  spec.files = `git ls-files -z`.split("\x0").reject do |f|
    f.match %r{^(test|spec|features)/}
  end

  spec.bindir      = 'exe'
  spec.executables = spec.files.grep %r{^exe/}, &File.method(:basename)

  spec.add_development_dependency 'bundler',   '~> 1.7'
  spec.add_development_dependency 'rake',      '~> 10.0'
  spec.add_development_dependency 'rubocop',   '~> 0.49.1'
  spec.add_development_dependency 'rspec',     '~> 3.3'
  spec.add_development_dependency 'simplecov', '~> 0.10'
  spec.add_development_dependency 'pry',       '~> 0.10'

  spec.add_runtime_dependency 'lita', '~> 4.7'
  spec.add_runtime_dependency 'tox',  '~> 0.0.2'
end
