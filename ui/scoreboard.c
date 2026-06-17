#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include<string.h>
#include<math.h>
#include"ui.h"
#include"../entity.h"
#include"color/color.h"

// for simplicity sake
//#define printf(...)	bprintf(INVALID_BUFFER_HANDLE, __VA_ARGS__)

#define BOX_CHAR		"▅"
#define ENERGY_DISP_PRECISION	20

#define max(x, y)	((x)>(y)?(x):(y))

extern Entity_t *Entities;
extern size_t EntityCount;
//extern Entity_t Players[3];

char *PadLeft(const char *str, size_t n, char ch) {
	size_t Length = strlen(str);
	size_t NewLength = max(n, Length);
	// add 1 for null terminator
	char *NewStr = malloc(NewLength+1);

	size_t StrOffset = NewLength - Length;
	memset(NewStr, ch, NewLength);
	memcpy(NewStr+StrOffset, str, Length);

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

size_t CalculateDigitsInNumber(uint64_t n) {
	size_t f = floor(max(log10(n)+1, 1));
	return f + ((f-1) / 3);//floor(max(log10(n)+1, 1));
}

char *IntToStr(uint64_t n) {
	size_t RequiredDigits = CalculateDigitsInNumber(n);//floor(max(log10(n)+1, 1));

//	printf("[debug] RequiredDigits for value %llu - %zu\n", n, RequiredDigits);
//	return NULL;

	char *StringifiedNumber = malloc(RequiredDigits+1);
//	printf("recieved pointer %p\n", StringifiedNumber);
	size_t Index = RequiredDigits;

	size_t DigitCount = 0;
	do {
		//printf("n = %llu; Index = %zu\n", n, Index);
		StringifiedNumber[--Index] = n % 10 + '0';
		n /= 10;

		//printf("n = %llu\n", n);
		
		if (Index == 0) break;

		//if (Index > RequiredDigits) {
		//	printf("breaking\n");
		//	break;
		//}
			//break;	
	
		DigitCount++;
		if (DigitCount % 3 == 0) {
			StringifiedNumber[--Index] = ',';
			//DigitCount++;
		}

		if (Index > RequiredDigits)
			break;
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

	for (size_t i = 0; i < EntityCount; i++) {
		Entity_t *t = &Entities[i];

//		printf("i = %p\n", t->Name);
		const char *Name = t->Name;
		size_t NameLength = strlen(Name);

		if (NameLength > MaximumNameLength)
			MaximumNameLength = NameLength;

		if (t->HealthPoints > MaximumHealth)
			MaximumHealth = t->HealthPoints;
	}

	size_t MaximumHealthDigits = CalculateDigitsInNumber(MaximumHealth);
	//printf("MaximumHealthDigits = %zu\n", MaximumHealthDigits);

	for (size_t i = 0; i < EntityCount; i++) {
		//printf("\x1b[2K");

		char *NameStr = PadRight(Entities[i].Name, MaximumNameLength, ' ');
		char *HpStr = IntToStr(Entities[i].HealthPoints);

		char *HealthStr = PadLeft(HpStr, MaximumHealthDigits, ' ');
		free(HpStr);
		
		printf("%s has %s hp [", NameStr, HealthStr);
		
		GetTerminalForegroundColorStr(0, 100, 255);
		for (int j = 0; j < MAX_ENERGY; j += MAX_ENERGY / ENERGY_DISP_PRECISION) {
			if (Entities[i].Energy > j)	printf("%s", BOX_CHAR);
			else				printf(" ");
		}
		ResetTerminalForegroundColorStr();
		printf("] (%d%%)\n", Entities[i].Energy);

		free(NameStr);
		free(HealthStr);
	}
	return 0;
}
