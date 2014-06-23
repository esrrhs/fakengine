#!/bin/sh

BUILDS="fuck_game_server_engine test tools"

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
