#include<stdbool.h>
#include<stdlib.h>
#include"strings.h"

#if __STDC_VERSION__ >= 202311L

#include<stdbit.h>

#endif

int Str_ReallocStr(String_t *str, size_t Codepoints, int PermitTrimming);

size_t CountByteLeadingOnes(uint8_t Byte) {
	size_t Count = 0;
	while ((Byte & 0x80) > 0) {
		Count++;
		Byte <<= 1;
	}

	return Count;
}

String_t Str_CreateString(void) {
	return (String_t){
		.StringIndicator=0xFF,
		.CodepointsAllocated=0,
		.Chars=NULL,
		.Length=0
	};
}

String_t Str_ToString(const char *str) {
	if (str == NULL) return (String_t){ 0 };
	String_t String = Str_CreateString();

	// todo: convert UTF-8 string to list of codepoints
	
	size_t CodepointBytes = 0;
	Codepoint_t Code = 0;

	str--;
	while (*(++str) != '\0') {
		size_t Ones = CountByteLeadingOnes(*str);

		if (Ones != 0) {
			Code = *str;
		} else {
			
		}

		// append codepoint
		Str_ReallocStr(&String, String.Length + 1, 0);
	}

	return Str_CreateString();
}

char *Str_FromString(String_t *str) {
	if (str == NULL) return NULL;

	// todo: convert list of codepoints to UTF-8 string
	return NULL;
}

void Str_DeleteString(String_t *str) {
	if (str == NULL) return;

	if (str->Chars != NULL) free(str->Chars);
	str->Length = 0;
}

int Str_ReallocStr(String_t *str, size_t Codepoints, int PermitTrimming) {
	if (str == NULL) return -1;

	if (PermitTrimming == false) {
		if (Codepoints < str->Length)
			return -1;
	}

	Codepoint_t *NewChars = realloc(str->Chars, Codepoints * sizeof(Codepoint_t));
	if (NewChars == NULL) return -1;

	str->Chars = NewChars;
	return 0;
}
