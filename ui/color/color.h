#pragma once

#include<stdint.h>

typedef struct Color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} Color_t;

enum TerminalColor {
	COLOR_MODE_FOREGROUND=0x0001,
	COLOR_MODE_BACKGROUND=0x0002,
	COLOR_MODE_RESET=0x0004,
};

// All color functions will always return an empty string to allow for use in printf statements.
const char *GetTerminalForegroundColorStr(uint8_t r, uint8_t g, uint8_t b);	// Get string for given forground color
const char *ResetTerminalForegroundColorStr();					// Get string to reset forground terminal color

const char *GetTerminalBackgroundColorStr(uint8_t r, uint8_t g, uint8_t b);	// Get string for given background color
const char *ResetTerminalBackgroundColorStr();					// Get string to reset background terminal color

const char *ModifyTerminalColor(enum TerminalColor Mode, Color_t clr);		// Generic terminal color function.
Color_t GetColorFromHex(uint32_t Color);
