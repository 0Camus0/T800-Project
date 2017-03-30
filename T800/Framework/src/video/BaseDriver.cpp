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

void BaseRT::LoadRT(int nrt, int cf, int df, int w, int h) {
	this->number_RT = nrt;
	this->color_format = cf;
	this->depth_format = df;
	this->w = w;
	this->h = h;
	LoadAPIRT();
}