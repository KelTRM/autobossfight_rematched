#include"../console_manager/console.h"
#include"color.h"

const char *GetTerminalForegroundColorStr(uint8_t r, uint8_t g, uint8_t b) {
	printf("\x1b[38;2;%d;%d;%dm", r, g, b);
	return "";
}

const char *ResetTerminalForegroundColorStr(void) {
	printf("\x1b[39m");
	return "";
}

