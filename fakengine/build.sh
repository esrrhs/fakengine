#!/bin/sh

find ./ -name CMakeCache.txt -delete
find ./ -name CMakeFiles | xargs rm -rf

cmake .
make -j8

ls -lh ../lib/libfengine.a
