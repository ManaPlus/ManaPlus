#!/bin/bash -x

for i in *.dylib
do
install_name_tool -id @executable_path/../SharedSupport/$i $i
done
