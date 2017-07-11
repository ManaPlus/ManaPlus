#!/bin/bash

export dir=$(pwd)
export ERRFILE=${dir}/logs/${LOGFILE}

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
        sed -i 's!http://deb.debian.org/debian!http://1.10.100.103/debian!' /etc/apt/sources.list
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
        echo "Running from shell. Skipping apt-get"
        return
    fi

    update_repos
    aptget_update

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

function gitclone1 {
    echo git clone $2
    git clone $2
    if [ "$?" != 0 ]; then
        echo git clone $1
        git clone $1
        return $?
    fi
    return $?
}

function gitclone {
    export name1=$1/$2
    export name2=${CI_BUILD_REPO##*@}
    export name2=https://${name2%/*}/$2

    gitclone1 "$name1" "$name2"
    if [ "$?" != 0 ]; then
        sleep 1s
        gitclone1 "$name1" "$name2"
        if [ "$?" != 0 ]; then
            sleep 3s
            gitclone1 "$name1" "$name2"
            if [ "$?" != 0 ]; then
                sleep 5s
                gitclone1 "$name1" "$name2"
                if [ "$?" != 0 ]; then
                    sleep 10s
                    gitclone1 "$name1" "$name2"
                    if [ "$?" != 0 ]; then
                        sleep 15s
                        gitclone1 "$name1" "$name2"
                        if [ "$?" != 0 ]; then
                            sleep 20s
                            gitclone1 "$name1" "$name2"
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
        cp config.log logs
        cp src/test-suite.log logs
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
    if [ "$JOBS" == "" ]; then
        export JOBS=2
        echo "No JOBS defined"
    fi
    echo make clean
    make clean || true
    echo "make -j${JOBS} V=0 $*"
    make -j${JOBS} V=0 $* 2>$ERRFILE
    check_error $?
}

function run_make_check {
    rm $ERRFILE
    if [ "$JOBS" == "" ]; then
        export JOBS=2
        echo "No JOBS defined"
    fi
    echo make clean
    make clean || true
    echo "make -j${JOBS} V=0 check $*"
    make -j${JOBS} V=0 check $* 2>$ERRFILE
    export ERR=$?
    if [ "${ERR}" != 0 ]; then
        cat $ERRFILE
        cp src/*.log logs
        cp src/manaplustests.trs logs
        cp src/Makefile logs
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
    export DATA=$(grep -A 2 "uninitialised" valg.log|grep ".cpp")
    if [ "$DATA" != "" ];
    then
        cat valg.log
        echo "valgrind error"
        exit 1
    fi
    cat valg.log
    echo "valgrind check"
}

function run_gcov {
    gcovr -r . --gcov-executable=$1 --html --html-details -o logs/$2.html
    check_error $?
    gcovr -r . --gcov-executable=$1 -o logs/$2.txt
    check_error $?
    cat logs/$2.txt
    check_error $?
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
    echo "$CC -c -x c++ $* $includes */*/*/*/*.h */*/*/*.h */*/*.h */*.h *.h"
    $CC -c -x c++ $* $includes */*/*/*/*.h */*/*/*.h */*/*.h */*.h *.h 2>$ERRFILE
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
    mplint/src/mplint $* \
    | grep -v "src/unittests/doctest.h" \
    | grep -v "src/unittests/catch.hpp" \
    | grep -v "src/debug/" \
    >$ERRFILE
    run_check_warnings
}
