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

#include <utils/Timer.h>

void Timer::Init() {
#ifdef OS_WINDOWS
	LARGE_INTEGER y;
	QueryPerformanceFrequency(&y);
	Frequency = double(y.QuadPart) / 1000000.0;
	StartTime.QuadPart = 0;
	Dt = 0.0;
	QueryPerformanceCounter(&StartTime);
#elif defined(OS_LINUX)

#endif
}

void Timer::Update() {
#ifdef OS_WINDOWS
	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);
	Dt = double(end.QuadPart - StartTime.QuadPart) / Frequency;
	QueryPerformanceCounter(&StartTime);
	DtSecs = (Dt / 1000000.0);
#elif defined(OS_LINUX)

#endif
}

float	Timer::GetDTSecs() {
#ifdef OS_WINDOWS
	return static_cast<float>(DtSecs);
#else
	return 1.0f/60.0f;
#endif
}
