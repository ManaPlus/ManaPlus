#!/bin/bash

export LOGFILE=bot.log
export name="ManaPlus"

source ./tools/ci/scripts/init.sh
source ./tools/ci/scripts/bot.sh

aptget_install ii psmisc

bot_connect "manaplusbot2"
bot_join "#manaplus"

export gitcommit=$(echo ${CI_BUILD_REF} | cut -c 1-7)

bot_message "#manaplus" "[${C}02${name}:${CI_BUILD_REF_NAME}${R}] Build success: ${C}03${gitcommit}${R}. See https://gitlab.com/manaplus/manaplus/builds"
bot_exit

exit 0
