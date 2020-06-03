#!/usr/bin/env bash

echo "Custom libs linked"
grep "spm" "$(ldd $1)"

search="$2"
if [[ "${search}" == "" ]]; then
    search="${LIBNAME}_${LIBVERSION}"
fi
res=$(ldd $1|grep "spm"|grep "$search")

if [[ "${res}" == "" ]]; then
    echo "Cant find requested lib: $search"
    exit 1
fi
exit 0
