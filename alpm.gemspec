# frozen_string_literal: true

require_relative 'lib/alpm/version'

Gem::Specification.new do |spec|
  spec.name          = 'alpm'
  spec.version       = ALPM::VERSION
  spec.authors       = ['Xuanrui Qi']
  spec.email         = ['me@xuanruiqi.com']

  spec.summary       = 'Ruby bindings for libalpm, the Arch Linux Package Management library'
  spec.homepage      = 'https://github.com/xuanruiqi/ruby-alpm'
  spec.licenses      = ['GPL-3.0-or-later']
  spec.required_ruby_version = Gem::Requirement.new('>= 2.4.0')

  spec.metadata['homepage_uri'] = spec.homepage
  spec.metadata['source_code_uri'] = 'https://github.com/xuanruiqi/ruby-alpm'
  spec.metadata['changelog_uri'] = 'https://github.com/xuanruiqi/ruby-alpm'

  # Specify which files should be added to the gem when it is released.
  # The `git ls-files -z` loads the files in the RubyGem that have been added into git.
  spec.files = Dir.chdir(File.expand_path(__dir__)) do
    `git ls-files -z`.split("\x0").reject { |f| f.match(%r{\A(?:test|spec|features)/}) }
  end

  spec.require_paths = ['lib']

  spec.extensions = %w[ext/alpm/extconf.rb]

  # Dependencies
  spec.add_dependency 'rake', '~> 13.0'
  spec.add_dependency 'rake-compiler', '~> 1.0.1'
  spec.add_development_dependency 'pry'
end
