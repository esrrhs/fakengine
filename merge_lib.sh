#!/usr/bin/env bash

LIBS="libfengine libmysqlclient-64"

rm merge_tmp -rf
mkdir merge_tmp > /dev/null 2>&1
cd merge_tmp

for i in ${LIBS}; do
	echo "x " $i
	mkdir $i > /dev/null 2>&1
	cd $i
	ar -x ../../lib/$i.a
	cd ..
	echo "x done " $i
done

OBJ=`find -name "*.o"`
echo $OBJ
ar -r libfengine.a $OBJ 
cp libfengine.a ../bin/

cd ..
rm merge_tmp -rf

echo "done"
