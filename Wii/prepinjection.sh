#!/usr/bin/env bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )





echo "Compiling dolinject..."
cd $SCRIPT_DIR/tools/dolinject

mkdir build
cd build

cmake -S .. -B . -G Ninja
ninja





echo "Generating makefile..."
cd $SCRIPT_DIR
$SCRIPT_DIR/tools/dolinject/build/dolinject compute-bases $1 Makefile-bases.autogen

