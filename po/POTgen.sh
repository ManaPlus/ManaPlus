#!/bin/bash

cd ..
grep "_(" -Irl src | sort >po/POTFILES.in
