#!/bin/sh

find ./ -name CMakeCache.txt -delete
find ./ -name CMakeFiles | xargs rm -r

cmake . $BUILD_FLAG
make clean
make -j
