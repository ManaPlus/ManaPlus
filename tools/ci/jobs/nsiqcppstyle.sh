#!/bin/bash

export LOGFILE=nsiqcppstyle.log

source ./tools/ci/scripts/init.sh

rm -rf nsiqcppstyle.git
./tools/ci/scripts/retry.sh git clone https://gitlab.com/4144/nsiqcppstyle nsiqcppstyle.git

./nsiqcppstyle.git/nsiqcppstyle --ci --show-url src \
| grep -v "catch.hpp" \
| grep -v "sdl2gfx" \
| grep -v "/debug/" \
>logs/nsiqcppstyle1.log

cat logs/nsiqcppstyle1.log \
| grep -v -E "gamemodifiers[.]cpp(.+):  Incorrect align on long parameter list in front of '(N_|{|})'" \
| grep -v -E "graphics_calcImageRect[.]hpp(.+):  function [(]calcTileVertexesInline[)] has non named parameter" \
| grep -v -E "dye[.]cpp(.+):  Do not use goto keyword" \
| grep -v -E "item/item[.]cpp(.+):  function [(]dragDrop[)] has non named parameter" \
| grep -v -E "windowmenu[.]cpp(.+):  function [(]addButton[)] has non named parameter" \
| grep -v -E "windowmenu[.]cpp(.+):  Incorrect align on long parameter list in front of '(.|Visible_true)', it should be aligen in column 15" \
| grep -v -E "channeltab[.]h(.+):  Incorrect align on long parameter list in front of 'const'" \
| grep -v -E "/enums/simpletypes/(.+):  function [(]defIntEnum[)] has non named parameter" \
>${ERRFILE}

export data=$(cat ${ERRFILE})
if [ "$DATA" != "" ]; then
    echo "Warnings detected"
    cat ${ERRFILE}
fi
