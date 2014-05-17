#!/bin/sh

find ./ -name CMakeCache.txt -delete
find ./ -name CMakeFiles | xargs rm -r

cmake .
rm libfengine.a
make

ls -lh libfengine.a
cp libfengine.a ../bin/
