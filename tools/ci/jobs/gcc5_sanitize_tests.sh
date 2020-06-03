#!/usr/bin/env bash

export CC=gcc-5
export CXX=g++-5

if [ "$NEWCC" != "" ]; then
    export CC="$NEWCC"
fi
if [ "$NEWCXX" != "" ]; then
    export CXX="$NEWCXX"
fi

export LOGFILE=gcc5_tests.log

source ./tools/ci/scripts/init.sh

export CXXFLAGS="-ggdb3 -O2 -ffast-math \
-D_GLIBCXX_SANITIZE_VECTOR \
-fsanitize=address -fsanitize=undefined \
-fsanitize=shift -fsanitize=integer-divide-by-zero -fsanitize=unreachable \
-fsanitize=vla-bound -fsanitize=null -fsanitize=return \
-fsanitize=signed-integer-overflow -fsanitize=bounds -fsanitize=alignment \
-fsanitize=object-size -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow \
-fsanitize=nonnull-attribute -fsanitize=returns-nonnull-attribute -fsanitize=bool \
-fsanitize=enum -fsanitize=vptr \
-fno-omit-frame-pointer -funswitch-loops \
-Wvariadic-macros -Wvla -Wredundant-decls \
-Wpacked-bitfield-compat -Wtrampolines \
-Wsuggest-attribute=noreturn -Wunused -Wstrict-aliasing=2 \
-fstrict-aliasing -Wunreachable-code \
-Wvolatile-register-var -Winvalid-pch -Wredundant-decls \
-Wnormalized=nfkc -Wmissing-format-attribute -Wmissing-noreturn \
-Wswitch-default -Wsign-promo -Waddress -Wmissing-declarations \
-Wctor-dtor-privacy -Wstrict-null-sentinel -Wlogical-op \
-Wcast-align -Wpointer-arith -Wundef \
-Wmissing-include-dirs -Winit-self -pedantic -Wall \
-Wpacked -Wextra -fstrict-overflow -Wstrict-overflow=1 -Wunknown-pragmas \
-Wwrite-strings -Wstack-protector -Wshadow -Wunused-macros -Wsynth \
-Wbuiltin-macro-redefined -Wctor-dtor-privacy -Wdeprecated \
-Wendif-labels -Wformat=1 -Wimport -Wnon-virtual-dtor -Wpsabi \
-Wsign-promo -Wwrite-strings -D_FORTIFY_SOURCE=2 -std=gnu++1z \
-Wdelete-non-virtual-dtor -Wmaybe-uninitialized -Wunused-local-typedefs \
-Wvector-operation-performance -Wfree-nonheap-object -Winvalid-memory-model \
-Wnarrowing -Wzero-as-null-pointer-constant -funsafe-loop-optimizations \
-Waggressive-loop-optimizations -Wclobbered -Wempty-body \
-Wignored-qualifiers -Wliteral-suffix -Wmissing-field-initializers \
-Woverlength-strings -Wpedantic -Wsign-compare -Wsizeof-pointer-memaccess \
-Wsuggest-attribute=format -Wtype-limits -Wuninitialized \
-Wunused-but-set-parameter -Wunused-but-set-variable -Wunused-function \
-Wunused-label -Wunused-parameter -Wunused-value -Wunused-variable -ftrapv \
-fsched-pressure -Wconditionally-supported -Wdate-time -fno-var-tracking \
-Wopenmp-simd -Wformat-signedness -Wformat-contains-nul \
-Wformat-extra-args -Wformat-security -Wswitch-bool -Wswitch-enum \
-Wformat-y2k -Wformat-zero-length -Wmemset-transposed-args -Wchkp \
-Wc++14-compat -Wsized-deallocation -Wlogical-not-parentheses \
-Woverloaded-virtual -Warray-bounds -Wbool-compare -Wchar-subscripts \
-Wcomment -Wmissing-braces -Wnonnull -Wopenmp-simd -Wparentheses \
-Wreturn-type -Wsequence-point -Wswitch \
-Wno-attributes"

export LDFLAGS="$LDFLAGS -lm -ldl"

do_init
run_configure --enable-unittests=yes $*
export SDL_VIDEODRIVER=dummy
export ASAN_OPTIONS=detect_leaks=0:detect_stack_use_after_return=true:strict_init_order=true
export LSAN_OPTIONS=suppressions=$(pwd)/tools/ci/scripts/lsansuppression_tests.txt
run_make_check

source ./tools/ci/scripts/exit.sh

exit 0
