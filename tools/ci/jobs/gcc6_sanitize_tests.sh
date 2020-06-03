#!/usr/bin/env bash

export CC=gcc-6
export CXX=g++-6

if [ "$NEWCC" != "" ]; then
    export CC="$NEWCC"
fi
if [ "$NEWCXX" != "" ]; then
    export CXX="$NEWCXX"
fi

export LOGFILE=gcc6.log

source ./tools/ci/scripts/init.sh

export CXXFLAGS="$CXXFLAGS -pedantic -ggdb3 -O2 -Wstrict-aliasing=2 \
-Wstrict-overflow=1 -Wformat=1 -D_FORTIFY_SOURCE=2 \
-fno-omit-frame-pointer \
-D_GLIBCXX_SANITIZE_VECTOR \
-fsanitize=address -fsanitize=undefined \
-fsanitize=shift -fsanitize=integer-divide-by-zero -fsanitize=unreachable \
-fsanitize=vla-bound -fsanitize=null -fsanitize=return \
-fsanitize=signed-integer-overflow -fsanitize=bounds -fsanitize=alignment \
-fsanitize=object-size -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow \
-fsanitize=nonnull-attribute -fsanitize=returns-nonnull-attribute -fsanitize=bool \
-fsanitize=enum -fsanitize=vptr -fsanitize=bounds-strict \
-std=gnu++1z -Wformat=1 \
-Wno-attributes"

export LDFLAGS="$LDFLAGS -lm -ldl"

#source ./tools/ci/flags/gcc6.sh

export CXXFLAGS="$CXXFLAGS $POST_CXXFLAGS"

do_init
run_configure $*
export SDL_VIDEODRIVER=dummy
export ASAN_OPTIONS=detect_leaks=0:detect_stack_use_after_return=true:strict_init_order=true
export LSAN_OPTIONS=suppressions=$(pwd)/tools/ci/scripts/lsansuppression_tests.txt
run_make_check

source ./tools/ci/scripts/exit.sh

exit 0
