#!/bin/sh

dbus-daemon --system --fork --nopidfile
daemonize /lib/systemd/systemd-journald

su bob -c "rm -rf build; meson setup build; cd build; meson compile"

su bob
