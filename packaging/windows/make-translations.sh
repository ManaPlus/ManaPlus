#!/bin/sh

for i in `grep -v ^# ../../po/LINGUAS`; do
    mkdir -p ../../translations/$i/LC_MESSAGES
    msgfmt -c -o ../../translations/$i/LC_MESSAGES/manaplus.mo ../../po/$i.po
done
