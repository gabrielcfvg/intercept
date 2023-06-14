#!/usr/bin/env bash
set -e

source any_newer_than.sh

build_type="Debug"

if (($# > 0));
then

    if (($# > 1));
    then
        echo "invalid number of parameters"
        exit 1
    fi

    build_type=$1
fi

if !([ -e "build" ]) || [ $build_type != "$(cat build/BUILD_TYPE)" ] || (any_newer_than build/intercept src CMakeLists.txt build.sh run.sh); then
    ./build.sh $build_type
    touch build/intercept
fi

./build/intercept