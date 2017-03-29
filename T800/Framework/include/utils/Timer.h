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

#ifndef UAD_TIMER_H
#define UAD_TIMER_H

#include <Config.h>

#include <windows.h>

class Timer {
public:
	void Init();
	void Update();
	
	float	GetDTSecs();
	
	LARGE_INTEGER	StartTime;
	double			Frequency;
	double			Dt;
	double			DtSecs;
};

#endif