#!/usr/bin/env bash

export CC=clang-5.0
export CXX=clang++-5.0

if [ "$NEWCC" != "" ]; then
    export CC="$NEWCC"
fi
if [ "$NEWCXX" != "" ]; then
    export CXX="$NEWCXX"
fi

export LOGFILE=clang50.log

source ./tools/ci/scripts/init.sh

# need fix -Wshadow-field

export CXXFLAGS="$CXXFLAGS -Weverything -Wno-documentation -Wno-padded -Wno-sign-conversion \
-Wno-global-constructors -Wno-exit-time-destructors -Wno-weak-vtables \
-Wno-overloaded-virtual -Wno-covered-switch-default -Wno-float-equal -Wno-conversion \
-Wno-shorten-64-to-32 -Wno-missing-variable-declarations \
-Wno-disabled-macro-expansion -Wno-format-nonliteral \
-Wno-reserved-id-macro -Wno-packed -Wno-documentation-unknown-command \
-Wno-variadic-macros -Wno-double-promotion -Wno-attributes -Wno-shadow-field \
-Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-implicit-fallthrough \
-Wpointer-bool-conversion -fno-omit-frame-pointer -ggdb3 $POST_CXXFLAGS"

do_init
run_configure --enable-werror $*
run_make

source ./tools/ci/scripts/exit.sh

exit 0
