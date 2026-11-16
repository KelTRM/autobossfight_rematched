#pragma once

#include<stdint.h>

// i'm just gonna take it from VGA 4-bit mode
enum Colors {
	COLOR_BLACK=0x000000,
	COLOR_BLUE=0x000088,
	COLOR_GREEN=0x008800,
	COLOR_CYAN=0x008888,
	COLOR_RED=0x880000,
	COLOR_MAGENTA=0x880088,
	COLOR_BROWN=0x886600,
	COLOR_LIGHTGRAY=0xBBBBBB,
	COLOR_DARKGRAY=0x777777,
	COLOR_LIGHTBLUE=0x0000FF,
	COLOR_LIGHTGREEN=0x00FF00,
	COLOR_LIGHTCYAN=0x00FFAA,
	COLOR_LIGHTRED=0xFF0000,
	COLOR_LIGHTMAGENTA=0xFF00FF,
	COLOR_YELLOW=0xFFFF00,
	COLOR_WHITE=0xFFFFFF
};

#define StaticColor(hex)		\
	((Color_t){			\
	 	.r=((hex)>>16)&0xFF,	\
		.g=((hex)>>8)&0xFF,	\
		.b=(hex)&0xFF		\
	})

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
const char *ResetTerminalForegroundColorStr(void);				// Get string to reset forground terminal color

const char *GetTerminalBackgroundColorStr(uint8_t r, uint8_t g, uint8_t b);	// Get string for given background color
const char *ResetTerminalBackgroundColorStr(void);				// Get string to reset background terminal color

const char *ModifyTerminalColor(enum TerminalColor Mode, Color_t clr);		// Generic terminal color function.
Color_t GetColorFromHex(uint32_t Color);

// Returns the actual escape sequence. Must be freed
char *GetColor(uint8_t Mode, Color_t clr);
char *GetClearColorStr(void);	// doesn't need to be freed, though

