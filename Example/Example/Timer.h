#ifndef UAD_TIMER_H
#define UAD_TIMER_H

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