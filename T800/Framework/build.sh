#!/bin/bash

BVERSION=$1

echo "Building Framework on [$BVERSION]"

rm -R obj/$BVERSION/*.o
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -O3 -Iinclude -I../Framework -I../Librerias/stb/include -I../Librerias/freeglut-linux/include -I../Librerias/glew-2.0.0/include -c src/core/Core.cpp -o obj/$BVERSION/Core.o
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -O3 -Iinclude -I../Framework -I../Librerias/stb/include -I../Librerias/freeglut-linux/include -I../Librerias/glew-2.0.0/include -c src/core/LinuxFramework.cpp -o obj/$BVERSION/LinuxFramework.o
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -O3 -Iinclude -I../Framework -I../Librerias/stb/include -I../Librerias/freeglut-linux/include -I../Librerias/glew-2.0.0/include -c src/scene/Cube.cpp -o obj/$BVERSION/Cube.o
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -O3 -Iinclude -I../Framework -I../Librerias/stb/include -I../Librerias/freeglut-linux/include -I../Librerias/glew-2.0.0/include -c src/scene/GLMesh.cpp -o obj/$BVERSION/GLMesh.o
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -O3 -Iinclude -I../Framework -I../Librerias/stb/include -I../Librerias/freeglut-linux/include -I../Librerias/glew-2.0.0/include -c src/scene/GLQuad.cpp -o obj/$BVERSION/GLQuad.o
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -O3 -Iinclude -I../Framework -I../Librerias/stb/include -I../Librerias/freeglut-linux/include -I../Librerias/glew-2.0.0/include -c src/scene/PrimitiveInstance.cpp -o obj/$BVERSION/PrimitiveInstance.o
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -O3 -Iinclude -I../Framework -I../Librerias/stb/include -I../Librerias/freeglut-linux/include -I../Librerias/glew-2.0.0/include -c src/scene/PrimitiveManager.cpp -o obj/$BVERSION/PrimitiveManager.o
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -O3 -Iinclude -I../Framework -I../Librerias/stb/include -I../Librerias/freeglut-linux/include -I../Librerias/glew-2.0.0/include -c src/scene/SceneProp.cpp -o obj/$BVERSION/SceneProp.o
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -O3 -Iinclude -I../Framework -I../Librerias/stb/include -I../Librerias/freeglut-linux/include -I../Librerias/glew-2.0.0/include -c src/scene/Triangle.cpp -o obj/$BVERSION/Triangle.o
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -O3 -Iinclude -I../Framework -I../Librerias/stb/include -I../Librerias/freeglut-linux/include -I../Librerias/glew-2.0.0/include -c src/utils/Camera.cpp -o obj/$BVERSION/Camera.o
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -O3 -Iinclude -I../Framework -I../Librerias/stb/include -I../Librerias/freeglut-linux/include -I../Librerias/glew-2.0.0/include -c src/utils/InputManager.cpp -o obj/$BVERSION/InputManager.o
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -O3 -Iinclude -I../Framework -I../Librerias/stb/include -I../Librerias/freeglut-linux/include -I../Librerias/glew-2.0.0/include -c src/utils/Timer.cpp -o obj/$BVERSION/Timer.o
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -O3 -Iinclude -I../Framework -I../Librerias/stb/include -I../Librerias/freeglut-linux/include -I../Librerias/glew-2.0.0/include -c src/utils/Utils.cpp -o obj/$BVERSION/Utils.o
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -O3 -Iinclude -I../Framework -I../Librerias/stb/include -I../Librerias/freeglut-linux/include -I../Librerias/glew-2.0.0/include -c src/utils/XDataBase.cpp -o obj/$BVERSION/XDataBase.o
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -O3 -Iinclude -I../Framework -I../Librerias/stb/include -I../Librerias/freeglut-linux/include -I../Librerias/glew-2.0.0/include -c src/utils/XMaths.cpp -o obj/$BVERSION/XMaths.o
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -O3 -Iinclude -I../Framework -I../Librerias/stb/include -I../Librerias/freeglut-linux/include -I../Librerias/glew-2.0.0/include -c src/video/BaseDriver.cpp -o obj/$BVERSION/BaseDriver.o
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -O3 -Iinclude -I../Framework -I../Librerias/stb/include -I../Librerias/freeglut-linux/include -I../Librerias/glew-2.0.0/include -c src/video/GLDriver.cpp -o obj/$BVERSION/GLDriver.o
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -O3 -Iinclude -I../Framework -I../Librerias/stb/include -I../Librerias/freeglut-linux/include -I../Librerias/glew-2.0.0/include -c src/video/GLRT.cpp -o obj/$BVERSION/GLRT.o
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -O3 -Iinclude -I../Framework -I../Librerias/stb/include -I../Librerias/freeglut-linux/include -I../Librerias/glew-2.0.0/include -c src/video/GLShader.cpp -o obj/$BVERSION/GLShader.o
g++ -std=c++11 -Wall -DOS_LINUX -DFREEGLUT_GLES -DFREEGLUT_WAYLAND -O3 -Iinclude -I../Framework -I../Librerias/stb/include -I../Librerias/freeglut-linux/include -I../Librerias/glew-2.0.0/include -c src/video/GLTexture.cpp -o obj/$BVERSION/GLTexture.o
rm -f bin/$BVERSION/libFramework.a
ar -r -s bin/$BVERSION/libFramework.a obj/$BVERSION/Core.o obj/$BVERSION/LinuxFramework.o obj/$BVERSION/Cube.o obj/$BVERSION/GLMesh.o obj/$BVERSION/GLQuad.o obj/$BVERSION/PrimitiveInstance.o obj/$BVERSION/PrimitiveManager.o obj/$BVERSION/SceneProp.o obj/$BVERSION/Triangle.o obj/$BVERSION/Camera.o obj/$BVERSION/InputManager.o obj/$BVERSION/Timer.o obj/$BVERSION/Utils.o obj/$BVERSION/XDataBase.o obj/$BVERSION/XMaths.o obj/$BVERSION/BaseDriver.o obj/$BVERSION/GLDriver.o obj/$BVERSION/GLRT.o obj/$BVERSION/GLShader.o obj/$BVERSION/GLTexture.o


