#!/usr/bin/env bash

export CC=gcc-4.9
export CXX=g++-4.9

if [ "$NEWCC" != "" ]; then
    export CC="$NEWCC"
fi
if [ "$NEWCXX" != "" ]; then
    export CXX="$NEWCXX"
fi

export LOGFILE=gcc49_tests.log

source ./tools/ci/scripts/init.sh

export CXXFLAGS="$CXXFLAGS -ggdb3 -O2 -ffast-math \
-D_GLIBCXX_SANITIZE_VECTOR \
-fsanitize=address \
-fno-omit-frame-pointer -funswitch-loops \
-Wvariadic-macros -Wvla -Wredundant-decls \
-Wpacked-bitfield-compat -Wtrampolines \
-Wsuggest-attribute=noreturn -Wunused -Wstrict-aliasing=2 \
-fstrict-aliasing -Wunreachable-code -Wabi \
-Wvolatile-register-var -Winvalid-pch -Wredundant-decls \
-Wnormalized=nfkc -Wmissing-format-attribute -Wmissing-noreturn \
-Wswitch-default -Wsign-promo -Waddress -Wmissing-declarations \
-Wctor-dtor-privacy -Wstrict-null-sentinel -Wlogical-op \
-Wcast-align -Wpointer-arith -Wundef \
-Wmissing-include-dirs -Winit-self -pedantic -Wall \
-Wextra -fstrict-overflow -Wstrict-overflow=1 -Wunknown-pragmas \
-Wwrite-strings -Wstack-protector -Wshadow -Wunused-macros -Wsynth \
-Wbuiltin-macro-redefined -Wctor-dtor-privacy -Wdeprecated \
-Wendif-labels -Wformat=1 -Wimport -Wnon-virtual-dtor -Wpsabi \
-Wsign-promo -Wwrite-strings -D_FORTIFY_SOURCE=2 -Wc++11-compat -std=c++0x \
-Wdelete-non-virtual-dtor -Wmaybe-uninitialized -Wunused-local-typedefs \
-Wvector-operation-performance -Wfree-nonheap-object -Winvalid-memory-model \
-Wnarrowing -Wzero-as-null-pointer-constant -funsafe-loop-optimizations \
-Wabi-tag -Waggressive-loop-optimizations -Wclobbered -Wempty-body \
-Wignored-qualifiers -Wliteral-suffix -Wmissing-field-initializers \
-Woverlength-strings -Wpedantic -Wsign-compare -Wsizeof-pointer-memaccess \
-Wsuggest-attribute=format -Wtype-limits -Wuninitialized \
-Wunused-but-set-parameter -Wunused-but-set-variable -Wunused-function \
-Wunused-label -Wunused-parameter -Wunused-value -Wunused-variable -ftrapv \
-fsched-pressure \
-Wconditionally-supported -Wdate-time -Woverloaded-virtual \
-fno-var-tracking $POST_CXXFLAGS"

export LDFLAGS="$LDFLAGS -lm -ldl"

do_init
run_configure --enable-unittests=yes $*
export SDL_VIDEODRIVER=dummy
run_make_check

source ./tools/ci/scripts/exit.sh

exit 0
