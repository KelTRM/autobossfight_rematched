#ifdef _WIN32
#include<windows.h>
#endif

#include<stdio.h>

int Init(void) {
	#ifdef _WIN32
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	#endif
	return 0;
}
