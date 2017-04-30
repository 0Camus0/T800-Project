#!/bin/bash

BVERSION=$1

echo "Building App on [$BVERSION]"

rm -R obj/$BVERSION/*.o
rm -R ../bin/AppLinux
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -O3 -I../Framework/include -I../Framework -I../Librerias/freeglut-linux/include -I../Librerias/stb/include -c App.cpp -o obj/$BVERSION/App.o
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -O3 -I../Framework/include -I../Framework -I../Librerias/freeglut-linux/include -I../Librerias/stb/include -c Application.cpp -o obj/$BVERSION/Application.o
g++ -L../Framework/bin/$BVERSION -L../Librerias/freeglut-linux/lib -o ../bin/AppLinux obj/$BVERSION/App.o obj/$BVERSION/Application.o  -s -lFramework -lfreeglut -lEGL -lGLESv2 -lwayland-cursor -lwayland-client -lwayland-egl -lxkbcommon  


