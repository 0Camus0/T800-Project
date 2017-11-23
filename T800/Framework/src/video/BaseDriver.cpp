/*********************************************************
* Copyright (C) 2017 Daniel Enriquez (camus_mm@hotmail.com)
* All Rights Reserved
*
* You may use, distribute and modify this code under the
* following terms:
* ** Do not claim that you wrote this software
* ** A mention would be appreciated but not needed
* ** I do not and will not provide support, this software is "as is"
* ** Enjoy, learn and share.
*********************************************************/
#include <video/BaseDriver.h>
#include <utils/cil.h>
#include <iostream>
#include <string>
#include <fstream>
#include <string.h>

namespace t800 {
  BaseDriver*	g_pBaseDriver = 0;
  Device*           T8Device;	// Device for create resources
  DeviceContext*    T8DeviceContext; // Context to set and manipulate the resources

#include <utils/Checker.h>

  bool		Texture::LoadTexture(const char *fn) {
    bool found = false;
    std::string path = "Textures/";
    std::string filepath = path + std::string(fn);
    std::ifstream inf(filepath.c_str());
    found = inf.good();
    inf.close();

    int x = 0, y = 0, channels = 0;
    unsigned char *buffer = 0;

    if (!found) {
      buffer = (unsigned char*)g_chkr.pixel_data;
      x = g_chkr.width;
      y = g_chkr.height;
      channels = g_chkr.bytes_per_pixel;
      std::cout << "Texture [" << filepath << "] not found, loading checker" << std::endl;
    }
    else {
      //buffer = stbi_load(filepath.c_str(), &x, &y, &channels, 0);
      cil_props = 0;
      buffer = cil_load((filepath.c_str()), &x, &y, &mipmaps, &cil_props, &size);
    }

    if (!buffer)
      return false;

    bounded = 1;
    this->x = x;
    this->y = y;
    this->params = params;
    props = 0;

    if (cil_props&CIL_RGBA) {
      props |= TEXT_BASIC_FORMAT::CH_RGBA;
      channels = 4;
    }
    else {
      props |= TEXT_BASIC_FORMAT::CH_RGB;
      channels = 3;
    }

    memcpy(&optname[0], fn, strlen(fn));
    optname[strlen(fn)] = '\0';

    LoadAPITexture(T8DeviceContext, buffer);
    if (found) {
      cil_free_buffer(buffer);
    }

    return true;
  }

  void Texture::DestroyTex() {
    DestroyAPITexture();
  }

  bool BaseRT::LoadRT(int nrt, int cf, int df, int w, int h, bool GenMips) {
    this->number_RT = nrt;
    this->color_format = cf;
    this->depth_format = df;
    this->w = w;
    this->h = h;
    this->GenMips = GenMips;
    return LoadAPIRT();
  }

  void BaseRT::DestroyRT() {
    DestroyAPIRT();
  }

  bool ShaderBase::CreateShader(std::string src_vs, std::string src_fs, unsigned int sig) {

    std::string Defines = "";

    bool LinearDepth = true;

#if defined(USING_OPENGL_ES20)
    LinearDepth = true; // Force for ES 2.0
#endif

#if defined(USING_OPENGL_ES30) || defined(USING_OPENGL_ES31)
    Defines += "#version 300 es\n\n";
    Defines += "#define ES_30\n\n";
#endif

#if VDEBUG_NO_LIGHT
    Defines += "#define NO_LIGHT\n\n";
#endif

#if VDEBUG_SIMPLE_COLOR
    Defines += "#define SIMPLE_COLOR\n\n";
#endif

    if (sig&Signature::HAS_NORMALS)
      Defines += "#define USE_NORMALS\n\n";
    if (sig&Signature::HAS_TEXCOORDS0)
      Defines += "#define USE_TEXCOORD0\n\n";
    if (sig&Signature::HAS_TEXCOORDS1)
      Defines += "#define USE_TEXCOORD1\n\n";
    if (sig&Signature::HAS_TANGENTS)
      Defines += "#define USE_TANGENTS\n\n";
    if (sig&Signature::HAS_BINORMALS)
      Defines += "#define USE_BINORMALS\n\n";
    if (sig&Signature::DIFFUSE_MAP)
      Defines += "#define DIFFUSE_MAP\n\n";
    if (sig&Signature::SPECULAR_MAP)
      Defines += "#define SPECULAR_MAP\n\n";
    if (sig&Signature::GLOSS_MAP)
      Defines += "#define GLOSS_MAP\n\n";
    if (sig&Signature::NORMAL_MAP)
      Defines += "#define NORMAL_MAP\n\n";
    if (sig&Signature::REFLECT_MAP)
      Defines += "#define REFLECT_MAP\n\n";
    if (sig&Signature::NO_LIGHT_AT_ALL)
      Defines += "#define NO_LIGHT\n\n";
    if (sig&Signature::GBUFF_PASS)
      Defines += "#define G_BUFFER_PASS\n\n";
    if (sig&Signature::FSQUAD_1_TEX)
      Defines += "#define FSQUAD_1_TEX\n\n";
    if (sig&Signature::FSQUAD_2_TEX)
      Defines += "#define FSQUAD_2_TEX\n\n";
    if (sig&Signature::FSQUAD_3_TEX)
      Defines += "#define FSQUAD_3_TEX\n\n";
    if (sig&Signature::SHADOW_MAP_PASS)
      Defines += "#define SHADOW_MAP_PASS\n\n";
    if (!LinearDepth)
      Defines += "#define NON_LINEAR_DEPTH\n\n";
    if (sig&Signature::SHADOW_COMP_PASS)
      Defines += "#define SHADOW_COMP_PASS\n\n";
    if (sig&Signature::DEFERRED_PASS)
      Defines += "#define DEFERRED_PASS\n\n";
    if (sig&Signature::VERTICAL_BLUR_PASS)
      Defines += "#define VERTICAL_BLUR_PASS\n\n";
    if (sig&Signature::HORIZONTAL_BLUR_PASS)
      Defines += "#define HORIZONTAL_BLUR_PASS\n\n";
    if (sig&Signature::ONE_PASS_BLUR)
      Defines += "#define ONE_PASS_BLUR\n\n";
    if (sig&Signature::BRIGHT_PASS)
      Defines += "#define BRIGHT_PASS\n\n";
    if (sig&Signature::HDR_COMP_PASS)
      Defines += "#define HDR_COMP_PASS\n\n";


    if (!LinearDepth)
      Defines += "#define NON_LINEAR_DEPTH\n\n";

#ifdef USING_OPENGL
    Defines += "#define lowp \n\n";
    Defines += "#define mediump \n\n";
    Defines += "#define highp \n\n";
#endif

#ifdef USING_16BIT_NORMALS
    Defines += "#define USING_16BIT_NORMALS\n\n";
#endif

    //	cout << "Compiling with the following signature[ " << Defines << endl << "]" << endl;

    src_vs = Defines + src_vs;
    src_fs = Defines + src_fs;
    this->Sig = sig;
    return CreateShaderAPI(src_vs, src_fs, sig);
  }
}
