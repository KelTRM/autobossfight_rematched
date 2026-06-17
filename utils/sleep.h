#pragma once

#ifdef _WIN32
#include<windows.h>
#define sleep(ms)	Sleep(ms);
#else
#include<unistd.h>
#define sleep(ms)	usleep((ms)*1000);
#endif
