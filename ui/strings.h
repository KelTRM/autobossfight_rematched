#pragma once

#include<stdint.h>
#include<stddef.h>

char *PadLeft(const char *str, size_t n, char ch);
char *PadRight(const char *str, size_t n, char ch);
size_t CalculateDigitsInNumber(uint64_t n);
char *IntToStr(uint64_t n);

#define GetLongestElementString(List, Name, ElementCount)			\
	GetLongestStringAtOffset(						\
			(char**)(List),						\
			sizeof(*List),						\
			ElementCount,						\
			(size_t)( (size_t)( &List->Name ) - (size_t)List )	\
	);

size_t GetLongestStringAtOffset(char **Elements, size_t ElementSize, size_t ElementCount, size_t Offset);
