#!/bin/bash

BVERSION=$1



if [ "$#" == 0 ];
then
echo "Default build is release..."
BVERSION="release"
fi

echo "Starting build... [$BVERSION]"

#rm -f ./bin/AppLinux

if [ "$BVERSION" == "debug" ];
then
echo "This is debug"
pushd ./Framework > /dev/null
./build.sh "debug"
popd > /dev/null
echo "\n\n"
pushd ./App > /dev/null
./build.sh "debug"
popd
pushd ./bin > /dev/null
chmod a+x ./AppLinux
popd > /dev/null
fi

if [ "$BVERSION" == "release" ];
then
echo "This is release"
pushd ./Framework > /dev/null
./build.sh "release"
popd > /dev/null
echo "\n\n"
pushd ./App > /dev/null
./build.sh "release"
popd > /dev/null
pushd ./bin > /dev/null
chmod a+x ./AppLinux
popd > /dev/null
fi

if [ ! -f ./bin/AppLinux ]; then
    echo "Something wrong, exe not generated"
else
   echo "Sucess on /bin/AppLinux"
fi



