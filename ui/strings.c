#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"strings.h"
#include"ui.h"

#define max(x, y)	((x)>(y)?(x):(y))

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

size_t GetLongestStringAtOffset(char **Elements, size_t ElementSize, size_t ElementCount, size_t Offset) {
	size_t Max = 0;

	for (size_t i = 0; i < ElementCount; i++) {
		size_t ElementOffset = i * ElementSize;
		size_t addr = (size_t)Elements + ElementOffset + Offset;
		char *str = *(char**)addr;

		size_t Length = strlen(str);

		if (Length > Max)
			Max = Length;

	}

	return Max;
}
