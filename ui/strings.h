#pragma once

#include<stdint.h>
#include<stddef.h>

char *PadLeft(const char *str, size_t n, char ch);
char *PadRight(const char *str, size_t n, char ch);
size_t CalculateDigitsInNumber(uint64_t n);
char *IntToStr(uint64_t n);
