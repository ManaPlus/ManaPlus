#!/bin/sh

cd ..
dpkg-buildpackage -rfakeroot -uc -b
