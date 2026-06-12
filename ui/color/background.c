#include"color.h"
#include"../console_manager/console.h"

const char *GetTerminalBackgroundColorStr(uint8_t r, uint8_t g, uint8_t b) {
	printf("\x1b[48;2;%d;%d;%dm", r, g, b);
	return "";
}

const char *ResetTerminalBackgroundColorStr() {
	printf("\x1b[49m");
	return "";
}
