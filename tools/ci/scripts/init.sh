#!/bin/bash

pwd
mkdir logs

export dir=$(pwd)
export ERRFILE=${dir}/logs/${LOGFILE}

cat /etc/os-release

rm ${ERRFILE}

function do_init {
    $CC --version
    check_error $?
    $CXX --version
    check_error $?
}

function update_repos {
    if [ "$RUNFROMSHELL" != "" ];
    then
        echo "Running from shell. Skipping update repos"
        return
    fi
    export DATA=$(cat /etc/resolv.conf|grep "nameserver 1.10.100.101")
    echo "${DATA}"
    if [ "$DATA" != "" ];
    then
        echo "Detected local runner"
        sed -i 's!http://httpredir.debian.org/debian!http://1.10.100.103/debian!' /etc/apt/sources.list
    fi
}

function aptget_update {
    if [ "$RUNFROMSHELL" != "" ];
    then
        echo "Running from shell. Skipping apt-get update"
        return
    fi
    echo "apt-get update"
    apt-get update
    if [ "$?" != 0 ]; then
        sleep 1s
        apt-get update
        if [ "$?" != 0 ]; then
            sleep 2s
            apt-get update
            if [ "$?" != 0 ]; then
                sleep 5s
                apt-get update
                if [ "$?" != 0 ]; then
                    sleep 10s
                    apt-get update
                    if [ "$?" != 0 ]; then
                        sleep 15s
                        apt-get update
                    fi
                fi
            fi
        fi
    fi
}

function aptget_install {
    if [ "$RUNFROMSHELL" != "" ];
    then
        echo "Running from shell. Skipping apt-get install"
        return
    fi
    echo "apt-get -y -qq install $*"
    apt-get -y -qq install $*
    if [ "$?" != 0 ]; then
        sleep 1s
        apt-get -y -qq install $*
        if [ "$?" != 0 ]; then
            sleep 2s
            apt-get -y -qq install $*
            if [ "$?" != 0 ]; then
                sleep 5s
                apt-get -y -qq install $*
                if [ "$?" != 0 ]; then
                    sleep 10s
                    aptget_update
                    apt-get -y -qq install $*
                    if [ "$?" != 0 ]; then
                        sleep 15s
                        apt-get -y -qq install $*
                    fi
                fi
            fi
        fi
    fi
}

function gitclone {
    git clone $*
    if [ "$?" != 0 ]; then
        sleep 1s
        git clone $*
        if [ "$?" != 0 ]; then
            sleep 3s
            git clone $*
            if [ "$?" != 0 ]; then
                sleep 5s
                git clone $*
                if [ "$?" != 0 ]; then
                    sleep 10s
                    git clone $*
                    if [ "$?" != 0 ]; then
                        sleep 15s
                        git clone $*
                        if [ "$?" != 0 ]; then
                            sleep 20s
                            git clone $*
                        fi
                    fi
                fi
            fi
        fi
    fi
    check_error $?
}

function check_error {
    if [ "$1" != 0 ]; then
        cat $ERRFILE
        exit $1
    fi
}

function run_configure_simple {
    rm $ERRFILE
    echo "autoreconf -i"
    autoreconf -i 2>$ERRFILE
    check_error $?
    rm $ERRFILE
    echo "./configure $*"
    ./configure $* 2>$ERRFILE
    check_error $?
}

function run_configure {
    run_configure_simple $*

    rm $ERRFILE
    cd po
    echo "make update-gmo"
    make update-gmo 2>$ERRFILE
    check_error $?
    cd ..

    rm $ERRFILE
    cd po
    echo "make update-po"
    make update-po 2>$ERRFILE
    check_error $?
    cd ..
}

function run_cmake {
    rm $ERRFILE
    echo "cmake ."
    cmake . 2>$ERRFILE
    check_error $?
}

function run_make {
    rm $ERRFILE
    echo "make -j2 V=0 $*"
    make -j2 V=0 $* 2>$ERRFILE
    check_error $?
}

function run_make_check {
    rm $ERRFILE
    echo "make -j2 V=0 check $*"
    make -j2 V=0 check $* 2>$ERRFILE
    export ERR=$?
    if [ "${ERR}" != 0 ]; then
        cat $ERRFILE
        cat src/manaplustests.log
        exit ${ERR}
    fi
    valgrind -q --read-var-info=yes --track-origins=yes --malloc-fill=11 --free-fill=55 --show-reachable=yes --leak-check=full --leak-resolution=high --partial-loads-ok=yes --error-limit=no ./src/manaplustests 2>valg.log
    export DATA=$(grep "invalid" valg.log)
    if [ "$DATA" != "" ];
    then
        cat valg.log
        echo "valgrind error"
        exit 1
    fi
    cat valg.log
    echo "valgrind check"
}

function run_check_warnings {
    DATA=$(cat $ERRFILE)
    if [ "$DATA" != "" ];
    then
        cat $ERRFILE
        exit 1
    fi
}

function run_h {
    rm $ERRFILE
    echo "$CC -c -x c++ $* $includes */*/*/*.h */*/*.h */*.h *.h"
    $CC -c -x c++ $* $includes */*/*/*.h */*/*.h */*.h *.h 2>$ERRFILE
    DATA=$(cat $ERRFILE)
    if [ "$DATA" != "" ];
    then
        cat $ERRFILE
        exit 1
    fi
}

function run_tarball {
    rm $ERRFILE
    echo "make dist-xz"
    make dist-xz 2>$ERRFILE
    check_error $?

    mkdir $1
    cd $1
    echo "tar xf ../*.tar.xz"
    tar xf ../*.tar.xz
    cd manaplus*
}

function run_mplint {
    rm $ERRFILE
    echo "mplint/src/mplint $*"
    mplint/src/mplint $* >$ERRFILE
    check_error $?
    run_check_warnings
}

update_repos
aptget_update
