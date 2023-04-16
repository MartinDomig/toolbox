#!/bin/sh

dbus-daemon --system --fork --nopidfile
daemonize /lib/systemd/systemd-journald

#set -- gosu bob "$@"
#exec "$@"
