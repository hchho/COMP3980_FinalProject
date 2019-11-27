#pragma once
#include <Windows.h>

struct Events {
	HANDLE receivedFrame{ 0 };
	HANDLE emptyBuffer{ 0 };

};