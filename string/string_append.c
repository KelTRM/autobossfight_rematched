#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include"strings.h"

int Str_ReallocStr(String_t *str, size_t Codepoints, int PermitTrimming);

String_t Str_Concat(String_t *a, String_t *b) {
	String_t String = Str_CreateString();

	size_t Length = a->Length + b->Length;
	Str_ReallocStr(&String, Length, 1);

	memcpy(String.Chars, a->Chars, a->Length);
	memcpy(String.Chars + a->Length, b->Chars, b->Length);

	return String;
}

String_t Str_Merge(size_t n, ...) {
	String_t String = Str_CreateString();

	String_t *DerivedList = NULL;
	size_t DerivedCount = 0;

	va_list args;	
	va_start(args, n);

	size_t TotalLength = 0;
	for (size_t i = 0; i < n; i++) {
		String_t *str = va_arg(args, String_t*);
		if (str == NULL) continue;	// idiot-proofing

		if (str->StringIndicator != 0xFF) {
			// it's a char*
			String_t *List=realloc(DerivedList,
					++DerivedCount * sizeof(String_t));

			if (List == NULL) {
				Str_DeleteString(&String);
				free(DerivedList);

				va_end(args);
				return (String_t){ 0 };
			}

			List[DerivedCount-1] = Str_ToString((const char*)str);
			DerivedList = List;

			str = List + DerivedCount-1;
		}

		TotalLength += str->Length;
	}

	va_end(args);

	int result = Str_ReallocStr(&String, TotalLength, 1);

	va_start(args, n);

	size_t CurrentOffset = 0;
	size_t DerivedIndex = 0;

	if (result >= 0) {
		// concatinate all the strings
		for (size_t i = 0; i < n; i++) {
			String_t *str = va_arg(args, String_t*);
			if (str == NULL) continue;

			if (str->StringIndicator != 0xFF)
				str = DerivedList + DerivedIndex;

			if (str == NULL) continue;

			memcpy(
				String.Chars + CurrentOffset,
				str->Chars,
				str->Length
			);

			CurrentOffset += str->Length;
		}
	} else {
		Str_DeleteString(&String);
	}

	if (DerivedList != NULL) {
		for (size_t i = 0; i < DerivedCount; i++) {
			Str_DeleteString(DerivedList + i);
		}
		free(DerivedList);
	}

	va_end(args);

	return String;
}
