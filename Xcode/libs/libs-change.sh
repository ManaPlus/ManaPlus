#!/bin/bash -x

#uncomment to get list of files
# for i in *.dylib
# do
# echo -change /opt/local/lib/$i @executable_path/../SharedSupport/$i
# done > changes


for i in *.dylib
do
install_name_tool `cat changes` $i
done
