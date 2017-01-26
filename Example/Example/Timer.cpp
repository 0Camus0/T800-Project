#include "Timer.h"

void Timer::Init() {
	LARGE_INTEGER y;
	QueryPerformanceFrequency(&y);
	Frequency = double(y.QuadPart) / 1000000.0;
	StartTime.QuadPart = 0;
	Dt = 0.0;
	QueryPerformanceCounter(&StartTime);
}

void Timer::Update() {
	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);
	Dt = double(end.QuadPart - StartTime.QuadPart) / Frequency;
	QueryPerformanceCounter(&StartTime);
	DtSecs = (Dt / 1000000.0);
}

float	Timer::GetDTSecs() {
	return static_cast<float>(DtSecs);
}