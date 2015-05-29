#!/bin/sh

find ./ -name CMakeCache.txt -delete
find ./ -name CMakeFiles | xargs rm -rf

cmake . $BUILD_FLAG
make -j

ls -lh ../lib/libfengine.a
