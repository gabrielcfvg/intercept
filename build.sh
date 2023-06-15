#!/usr/bin/env bash
set -e


if !([ -e "vcpkg" ]); then
    echo "uninitialized project"
    exit 1
fi

if (($# < 1)) || (($# > 1)); then
    echo "invalid number of parameters, usage: build.sh <build type>"
    exit 1
fi

build_type=$1

if [ $build_type != "Debug" ] && [ $build_type != "Release" ]; then
    echo "invalid build mode"
    exit 1
fi

cmake -B build -S . -DCMAKE_BUILD_TYPE=$build_type
cmake --build build -j`getconf _NPROCESSORS_ONLN`
echo -n $build_type > build/BUILD_TYPE
