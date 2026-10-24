#include<stdio.h>
#include<stdlib.h>

char *ReadFileAsStr(const char *Filename) {
	FILE *f = fopen(Filename, "r");

	if (f == NULL)
		return NULL;

	fseek(f, 0, SEEK_END);
	size_t Size = ftell(f);

	char *Buffer = malloc(Size+1);
	if (Buffer == NULL)
		return NULL;

	fseek(f, 0, SEEK_SET);
	fread(Buffer, 1, Size, f);

	Buffer[Size] = 0;

	fclose(f);

	return Buffer;
}

