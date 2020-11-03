#!/bin/bash
BUILD_TYPE=$1
tr [a-z] [A-Z] < $BUILD_TYPE

if [ "$BUILD_TYPE" = "DEBUG" ]; then
    BUILD_TYPE = "Debug"
else
    BUILD_TYPE = "Release"
fi

BUILD_DIR="build_"$1

cd ..;
mkdir $BUILD_DIR;
cd $BUILD_DIR;
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..

