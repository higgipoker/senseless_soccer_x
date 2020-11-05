#!/bin/bash

BUILD_TYPE=$1

if [ -z "$BUILD_TYPE" ]
  then
    BUILD_TYPE="debug"
fi

BUILD_TYPE=${BUILD_TYPE^^}

if [ "$BUILD_TYPE" = "RELEASE" ]; then
    BUILD_TYPE="Release"
else
    BUILD_TYPE="Debug"
fi

BUILD_DIR="build_"$BUILD_TYPE

cd ../bin/$BUILD_TYPE/;

./senseless_soccer3

