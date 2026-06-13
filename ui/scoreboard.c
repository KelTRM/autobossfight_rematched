#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include<string.h>
#include<math.h>
#include"ui.h"
#include"../entity.h"

// for simplicity sake
//#define printf(...)	bprintf(INVALID_BUFFER_HANDLE, __VA_ARGS__)

#define max(x, y)	((x)>(y)?(x):(y))

extern Entity_t Players[3];

char *PadLeft(const char *str, size_t n, char ch) {
	size_t Length = strlen(str);
	size_t NewLength = max(n, Length);
	// add 1 for null terminator
	char *NewStr = malloc(NewLength+1);

	size_t StrOffset = NewLength - Length;
	memset(NewStr+StrOffset, Length, ch);
	memcpy(NewStr, str, Length);

	NewStr[NewLength] = '\0';
	return NewStr;
}

char *PadRight(const char *str, size_t n, char ch) {
	size_t Length = strlen(str);
	size_t NewLength = max(n, Length);
	// add 1 for null terminator
	char *NewStr = malloc(NewLength+1);
	memset(NewStr, ch, NewLength);
	memcpy(NewStr, str, Length);

	NewStr[NewLength] = '\0';
	return NewStr;
}

char *IntToStr(uint64_t n) {
	size_t RequiredDigits = floor(max(log10(n)+1, 1));

//	printf("[debug] RequiredDigits for value %llu - %zu\n", n, RequiredDigits);
//	return NULL;

	char *StringifiedNumber = malloc(RequiredDigits+1);
//	printf("recieved pointer %p\n", StringifiedNumber);
	size_t Index = RequiredDigits;

	do {
//		printf("n = %llu; Index = %zu\n", n, Index);
		StringifiedNumber[--Index] = n % 10 + '0';
		n /= 10;
	} while (Index > 0);

	StringifiedNumber[RequiredDigits] = '\0';

	return StringifiedNumber;
}

int RefreshScoreboard(int PreserveCursorPosition) {
	if (PreserveCursorPosition)
		printf("\x1b[s");
	printf("\x1b[H");

	size_t MaximumNameLength = 0;
	Health_t MaximumHealth = 0;

	for (size_t i = 0; i < sizeof(Players) / sizeof(*Players); i++) {
		Entity_t *t = &Players[i];

//		printf("i = %p\n", t->Name);
		const char *Name = t->Name;
		size_t NameLength = strlen(Name);

		if (NameLength > MaximumNameLength)
			MaximumNameLength = NameLength;

		if (t->HealthPoints > MaximumHealth)
			MaximumHealth = t->HealthPoints;
	}

	for (size_t i = 0; i < sizeof(Players) / sizeof(*Players); i++) {
		printf("\x1b[2K");

		char *namestr = PadRight(Players[i].Name, MaximumNameLength, ' ');
		char *hpstr = IntToStr(Players[i].HealthPoints);
		printf("%s has %s hp\n", namestr, hpstr);

		free(namestr);
		free(hpstr);
	}
	return 0;
}
