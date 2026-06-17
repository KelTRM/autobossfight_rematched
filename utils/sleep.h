#pragma once

#ifdef __WIN32
#include<Windows.h>
#define sleep(ms)	Sleep(ms);
#else
#include<unistd.h>
#define sleep(ms)	usleep((ms)*1000);
#endif
