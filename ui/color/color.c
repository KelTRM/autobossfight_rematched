#include"color.h"
#include"../console_manager/console.h"

const char *ResetTerminalColor(enum TerminalColor Mode) {
	int Foreground = 0;
	int Background = 0;

	if (Mode & COLOR_MODE_FOREGROUND)
		Foreground = 39;
	if (Mode & COLOR_MODE_BACKGROUND)
		Background = 49;

	if (Foreground == Background && Background == 0)
		return "";

	int FirstParam = Foreground;
	int SecondParam = 0;

	if (Foreground == 0)
		FirstParam = Background;
	else if (Background != 0)
		SecondParam = Background;

	printf("\x1b[%d", FirstParam);
	if (SecondParam != 0) printf(";%d", SecondParam);
	printf("m");

	return "";
}

const char *ModifyTerminalColor(enum TerminalColor Mode, Color_t clr) {
	if (Mode & COLOR_MODE_RESET) {
		return ResetTerminalColor(Mode);
	}

	if (Mode & COLOR_MODE_FOREGROUND) {
		return GetTerminalBackgroundColorStr(clr.r, clr.g, clr.b);
	}

	if (Mode & COLOR_MODE_BACKGROUND) {
		return GetTerminalBackgroundColorStr(clr.r, clr.g, clr.b);
	}

	return "";
}
