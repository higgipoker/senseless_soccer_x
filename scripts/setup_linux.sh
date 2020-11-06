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

cd ..;
mkdir $BUILD_DIR;
cd $BUILD_DIR;
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..

# copy sfml libs
mkdir ../bin
mkdir ../bin/$BUILD_TYPE
cp -rvf ../sfml/lib/* ../bin/$BUILD_TYPE/

#copy resources
cp -rvf ../fonts ../bin/$BUILD_TYPE/

