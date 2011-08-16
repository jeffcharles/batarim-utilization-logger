#!/bin/bash

OLD_DIR=$( pwd )

# change directory to cmake_scripts
cd $( dirname "$0" )

cd ..
mkdir build

cd build
mkdir debug
mkdir release

cd debug
cmake -DCMAKE_BUILD_TYPE=Debug ../..
cd ..

cd release
cmake -DCMAKE_BUILD_TYPE=Release ../..
cd ..

cd $OLD_DIR
