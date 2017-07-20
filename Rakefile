# frozen_string_literal: true

require 'bundler/gem_tasks'

GEMSPEC = Gem::Specification.load 'lita-tox.gemspec'

github_user, github_project =
  GEMSPEC.homepage.scan(%r{^https://github\.com/([^/]+)/([^/]+)/?$})[0]

task default: %i[spec lint]

task lint: :rubocop

task fix: 'rubocop:auto_correct'

begin
  require 'rspec/core/rake_task'
  RSpec::Core::RakeTask.new
rescue LoadError
  nil
end

begin
  require 'rubocop/rake_task'
  RuboCop::RakeTask.new
rescue LoadError
  nil
end

desc 'Generate changelog'
task :changelog, [:token] do |_t, args|
  raise 'please provide access token' unless args[:token]
  sh "github_changelog_generator -u #{github_user} -p #{github_project} -t #{args[:token]}"
end
