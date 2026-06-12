#include"color.h"

const char *ModifyTerminalColor(enum TerminalColor Mode, Color_t clr) {
	if (Mode & COLOR_MODE_FOREGROUND) {
		if (Mode & COLOR_MODE_RESET)
			return ResetTerminalForegroundColorStr();
		return GetTerminalBackgroundColorStr(clr.r, clr.g, clr.b);
	}

	if (Mode & COLOR_MODE_BACKGROUND) {
		if (Mode & COLOR_MODE_RESET)
			return ResetTerminalBackgroundColorStr();
		return GetTerminalBackgroundColorStr(clr.r, clr.g, clr.b);
	}

	return "";
}
