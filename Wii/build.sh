#!/usr/bin/env bash

set -oe pipefail

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )



echo "Compiling dolinject..."
cd $SCRIPT_DIR/tools/dolinject

mkdir -p build
cd build

cmake -S .. -B . -G Ninja
ninja



echo "Generating makefile..."
cd $SCRIPT_DIR
$SCRIPT_DIR/tools/dolinject/build/dolinject compute-bases $1 Makefile-bases.autogen



echo "Making..."
make -f Makefile-bases.autogen clean
make -f Makefile-bases.autogen SHELL="sh -x"


echo "Injecting..."
$SCRIPT_DIR/tools/dolinject/build/dolinject inject $1 $2 $3