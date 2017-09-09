#!/usr/bin/env bash
apt-get install redis-server libhiredis-dev
rm CMakeCache.txt
PREFIX=/usr
cd hiredis
make
make install
cd ..
cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr . && make all install
