#!/bin/bash

autoreconf -i
./configure
cd po
make update-gmo
cd ..
make
