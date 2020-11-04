#!/bin/bash

BUILD_TYPE=$1

if [ -z "$BUILD_TYPE" ]
  then
    BUILD_TYPE="debug"
fi

tr [a-z] [A-Z] < $BUILD_TYPE

if [ "$BUILD_TYPE" = "RELEASE" ]; then
    BUILD_TYPE = "Release"
else
    BUILD_TYPE = "Debug"
fi

BUILD_DIR="build_"$BUILD_TYPE

cd ..;
mkdir $BUILD_DIR;
cd $BUILD_DIR;
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..

