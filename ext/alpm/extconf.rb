# frozen_string_literal: true

require 'mkmf'

if !(find_header 'alpm.h') || !(find_library 'alpm', 'alpm_version')
  abort <<~ERR
    libalpm was not found. Please make sure
    that you are building this Gem on an Arch Linux
    installation with pacman properly installed. If
    not, you may have to reinstall your system.
  ERR
end

create_makefile('alpm/alpm')
