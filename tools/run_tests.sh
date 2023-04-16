#!/bin/sh

. /prepare_tests.sh

cd /builds/WV/wvcore
rm -rf build
mkdir build
cd build
meson setup .. -Dbuildtype=debug -Db_coverage=true -Dtests=true -Dfunction-tests=true
meson compile
meson install
meson test

/bin/bash
