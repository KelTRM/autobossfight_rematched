#pragma once

/*
 * String_t - A string struct using UTF-32
 * Supports converting to/from UTF-8 char-array strings
 *
 * Implements plenty of string operations for ease under Str_* functions.
 * All returned strings must be freed using Str_DeleteString
 *
 */

#include<stdint.h>
#include<stddef.h>

typedef uint32_t Codepoint_t;

typedef struct {
	uint8_t StringIndicator;	// valid string has indicator of 0xFF
	size_t CodepointsAllocated;
	Codepoint_t *Chars;
	size_t Length;
} String_t;

String_t Str_CreateString(void);			// Creates an empty string
String_t Str_ToString(const char*);			// Converts a C-string to a String_t
char *Str_FromString(String_t*);			// Generates a C-string from a String_t

Codepoint_t Str_AtIndex(String_t*);

String_t Str_Concat(String_t*, String_t*);		// Concatinates 2 strings
String_t Str_Merge(size_t n, ...);			// Concatinates all n strings into one string
String_t Str_Copy(String_t*);

void Str_DeleteString(String_t*);			// Deletes the string

// All the ui/strings.h functions, using String_t

String_t Str_PadLeft(String_t*, size_t Length, char With);	// Pads a string to length at the start
String_t Str_PadRight(String_t*, size_t Length, char With);	// Pads a string to length at the end

// originally an internal function for ui/strings.h, but i'm keeping it here because it's useful
//size_t CalculateDigitsInNumber(uint64_t n, int Radix);	// Returns the base-radix digits within a number

String_t Str_IntToStrU64(uint64_t n, int Radix);	// Returns (unsigned) n as a base-radix string
String_t Str_IntToStrS64(int64_t n, int Radix);		// Returns (signed) n as a base-radix string

// #if so _Generic can be used with newer stdc versions. assume signed on older stdc
#if __STDC_VERSION__ >= 201112L

#define IntToStr(n,radix)	(_Generic((n),		\
	int64_t:	IntToS64(n,radix)		\
	uint64_t:	IntToU64((n),(radix))		\
	default:	NULL				\
))

#else

#define IntToStr(n,radix)	(IntToStrS64((n),(radix)))

#endif

