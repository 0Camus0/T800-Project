#!/bin/bash

BVERSION=$1


if [ "$#" == 0 ];
then
BVERSION="release"
fi

echo "Starting build... [$BVERSION]"

#rm -f ./bin/AppLinux

pushd ./Framework > /dev/null
./build.sh $BVERSION
popd > /dev/null
echo "\n\n"
pushd ./App > /dev/null
./build.sh $BVERSION
popd > /dev/null
pushd ./bin > /dev/null
chmod a+x ./AppLinux
popd > /dev/null


