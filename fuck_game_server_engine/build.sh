#!/bin/sh

find ./ -name CMakeCache.txt -delete
find ./ -name CMakeFiles | xargs rm -rf

cmake .
rm libfengine.a
make -j8

ls -lh libfengine.a
cp libfengine.a ../bin/
