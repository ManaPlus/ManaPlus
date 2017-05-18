#!/bin/bash

export LOGFILE=cpplint.log

source ./tools/ci/scripts/init.sh

rm cpplint.py
tools/ci/scripts/retry.sh wget "https://raw.githubusercontent.com/google/styleguide/gh-pages/cpplint/cpplint.py"
chmod +x cpplint.py

CHECKER="./cpplint.py --filter=\
-build/include,\
-whitespace/braces,-whitespace/operators,-runtime/references,\
-runtime/threadsafe_fn,-readability/streams,\
-runtime/rtti,\
-whitespace/newline,-whitespace/labels,\
-runtime/indentation_namespace,\
-whitespace/indent,\
-whitespace/parens,\
-runtime/int,\
-readability/todo,\
-runtime/arrays,\
-runtime/printf,\
-whitespace/empty_loop_body,\
-build/c++11,\
-readability/fn_size,\
-build/header_guard,\
-readability/braces,\
-legal/copyright,\
-whitespace/semicolon,\
-readability/namespace,\
-readability/casting,\
-readability/inheritance,\
-runtime/string,\
-build/c++tr1"

find src -type f -name "*.cpp" -exec $CHECKER {} \; 2>${ERRFILE}2
find src -type f -name "*.hpp" -exec $CHECKER {} \; 2>>${ERRFILE}2
find src -type f -name "*.h" -exec $CHECKER {} \; 2>>${ERRFILE}2
find src -type f -name "*.cc" -exec $CHECKER {} \; 2>>${ERRFILE}2

grep ":  " ${ERRFILE}2 | \
grep -v "debug_new" | \
grep -v "test/doctest.h" | \
grep -v "debug/fast_mutex" | \
grep -v "sdl2gfx/SDL2" \
>${ERRFILE}

rm ${ERRFILE}2

run_check_warnings

echo ok
