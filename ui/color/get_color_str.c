#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"color.h"

#define SEQUENCE_MAX_LENGTH		22

char *GetColor(uint8_t Mode, Color_t clr) {
	char Sequence[22];
	memset(Sequence, 0, 22);
	size_t CharCount = 0;

	snprintf(Sequence, 22, "\x1b[%d;2;%d;%d;%dm", Mode, clr.r, clr.g, clr.b);
	size_t Length = strnlen(Sequence, 22);

	char *str = malloc(Length+1);
	memcpy(str, Sequence, Length);
	str[Length] = 0;
	return str;
}
