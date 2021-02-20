# ALPM bindings for Ruby

This is the `alpm` Gem, currently under development, which is a binding to the Arch Linux Package Management 
([libalpm](https://git.archlinux.org/pacman.git/tree/lib/libalpm)) library.

## Installation/Usage

Currently this is in an early development stage, so I make no guarantees that you can install or use it. 
But you can use `rake compile`, `rake build` and `bin/console` to play with it.

## Development

After checking out the repo, run `bin/setup` to install dependencies. You will need to use `rake compile` to compile the native extensions.
You can also run `bin/console` for an interactive prompt that will allow you to experiment.

To install this gem onto your local machine, run `bundle exec rake install`.

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/xuanruiqi/ruby-alpm. This project is intended to be a safe, welcoming space for collaboration, and contributors are expected to adhere to the [code of conduct](https://github.com/xuanruiqi/ruby-alpm/blob/master/CODE_OF_CONDUCT.md).

## Code of Conduct

Everyone interacting in the Alpm project's codebases, issue trackers, chat rooms and mailing lists is expected to follow the [code of conduct](https://github.com/xuanruiqi/ruby-alpm/blob/master/CODE_OF_CONDUCT.md).

## License

The code is open-sourced under the GNU General Public License v3.0.
