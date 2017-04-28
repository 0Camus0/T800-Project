#!/bin/bash

BVERSION=$1

echo "Building App on [$BVERSION]"

if [ "$BVERSION" == "release" ]; then
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -O3 -I../Framework/include -I../Framework -I../Librerias/freeglut-linux/include -I../Librerias/stb/include -c /home/Camus/Dev/T800-Cloned/T800-Project/T800/App/App.cpp -o obj/Release/App.o
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -O3 -I../Framework/include -I../Framework -I../Librerias/freeglut-linux/include -I../Librerias/stb/include -c /home/Camus/Dev/T800-Cloned/T800-Project/T800/App/Application.cpp -o obj/Release/Application.o
g++ -L../Framework/bin/Release -L../Librerias/freeglut-linux/lib -o ../bin/AppLinux obj/Release/App.o obj/Release/Application.o  -s -lFramework -lfreeglut -lEGL -lGLESv2 -lwayland-cursor -lwayland-client -lwayland-egl -lxkbcommon  
fi

if [ "$BVERSION" == "debug" ]; then
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -g -I../Framework/include -I../Framework -I../Librerias/freeglut-linux/include -I../Librerias/stb/include -c /home/Camus/Dev/T800-Cloned/T800-Project/T800/App/App.cpp -o obj/Debug/App.o
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -g -I../Framework/include -I../Framework -I../Librerias/freeglut-linux/include -I../Librerias/stb/include -c /home/Camus/Dev/T800-Cloned/T800-Project/T800/App/Application.cpp -o obj/Debug/Application.o
g++ -L../Framework/bin/Debug -L../Librerias/freeglut-linux/lib -o ../bin/AppLinux obj/Debug/App.o obj/Debug/Application.o  -lFramework -lfreeglut -lEGL -lGLESv2 -lwayland-cursor -lwayland-client -lwayland-egl -lxkbcommon  
fi

if [ "$BVERSION" == "" ];
then
echo "Sorry? I don't get what kind of release you want.. choose release or debug then."
fi
