#!/usr/bin/env bash
set -e

BUILD_TYPE="Debug"
if [ "$1" = "release" ] || [ "$1" = "Release" ]; then
    BUILD_TYPE="Release"
fi

# Ensure CC and CXX point to matching compilers
export CC="${CC:-gcc}"
export CXX="${CXX:-g++}"

echo "==> Configuring with CMake ($BUILD_TYPE)..."
cmake -B build -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

echo "==> Building all targets..."
cmake --build build -j"$(nproc 2>/dev/null || echo 4)"

echo "==> Merging distributed libraries into bin/libfengine.a..."
./merge_lib.sh

echo "==> Cleaning shared memory leftovers..."
(cd bin && ./shmclean)

echo "==> Running automated test suite (CTest)..."
ctest --test-dir build --output-on-failure

echo "----------------All build & tests OK----------------"
