#ifndef UAD_CONFIG_H
#define UAD_CONFIG_H

#define D3DX 1
#define OGLES 2

#define DRIVER_SELECTED OGLES

#if   DRIVER_SELECTED == OGLES
#define USING_OPENGL_ES
#elif DRIVER_SELECTED == D3DX
#define USING_D3D11
#endif

#endif