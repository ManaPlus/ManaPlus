#!/bin/bash

DIR=$(dirname "$(readlink -f $0)")
cd "$DIR"
export LD_LIBRARY_PATH="$DIR"/libs/:$LD_LIBRARY_PATH
./bin/manaplus
