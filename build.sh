#!/bin/bash
buildargs="";
build(){
  dir=`pwd`

  autoreconf -i
  ./configure --prefix=$dir/run \
  --enable-commandlinepassword \
  --datadir=$dir/run/share/games \
  --bindir=$dir/run/bin \
  --mandir=$dir/run/share/man \
  --enable-portable=yes

  cd po
  make update-gmo
  cd ..
  make -j4 $1

  if [ ! -d "run" ]; then
      mkdir run
  fi

  make install
}

args=("$@")
for (( i=0; i <= $#; i++ )); do
  case ${args[$i]} in
    "-d")
    buildargs="-Wno-deprecated-declarations $buildargs"
        ;;
  esac
done
echo "added '$buildargs'"
build $buildargs
