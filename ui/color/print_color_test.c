#include<stdlib.h>
#include"color.h"
#include"../console_manager/console.h"

#define ArrayLen(arr)	(sizeof((arr))/sizeof(*(arr)))

uint32_t Colors[] = {
	COLOR_BLACK,
	COLOR_BLUE,//=0x000088,
	COLOR_GREEN,//=0x008800,
	COLOR_CYAN,//=0x008888,
	COLOR_RED,//=0x880000,
	COLOR_MAGENTA,//=0x880088,
	COLOR_BROWN,//=0x888800,
	COLOR_LIGHTGRAY,//=0xAAAAAA,
	COLOR_DARKGRAY,//=0x888888,
	COLOR_LIGHTBLUE,//=0x0000FF,
	COLOR_LIGHTGREEN,//=0x00FF00,
	COLOR_LIGHTCYAN,//=0x00FFAA,
	COLOR_LIGHTRED,//=0xFF0000,
	COLOR_LIGHTMAGENTA,//=0xFF00FF,
	COLOR_YELLOW,//=0xFFFF00,
	COLOR_WHITE,//=0xFFFFFF
};

int ColorTest(void) {
	for (size_t i = 0; i < ArrayLen(Colors); i++) {
		char *CurrentColor = GetColor(48, GetColorFromHex(Colors[i]));
		const char *Clear = "\x1b[49m";

		printf("%s   %s", CurrentColor, Clear);

		free(CurrentColor);
	}

	printf("\n");
	return 0;
}
