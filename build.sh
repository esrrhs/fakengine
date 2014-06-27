#!/bin/sh

# build lib
cd fuck_game_server_engine
./build.sh
if [ $? -ne 0 ];then
	echo "----------------build libfengine.a fail----------------"
	exit 1
fi
cd ..

#merge
./merge_lib.sh
if [ $? -ne 0 ];then
	echo "merge fail"
exit 1
fi

BUILDS="test tools"

for tobuild in $BUILDS; do
    cd $tobuild
	./build.sh
    if [ $? -ne 0 ];then
        echo "----------------build $tobuild fail----------------"
        exit 1
    fi
	cd ..
done    

cd bin
./shmclean

echo "----------------All build OK----------------"
