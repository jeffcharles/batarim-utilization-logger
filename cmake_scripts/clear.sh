#!/bin/bash

OLD_DIR=$( pwd )

# change directory to cmake_scripts
cd $( dirname "$0" )

rm -rf ../build

cd $OLD_DIR
