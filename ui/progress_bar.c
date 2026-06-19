#include <stdlib.h>
#include<string.h>
#include"console_manager/console.h"
#include"color/color.h"
#include"ui.h"

#define EMPTY_CHAR	" "
#define OCCUPIED_CHAR	BOX_CHAR

size_t AppendStrings(const char *str1, const char *str2, char **buffer, size_t BufferSize) {
	if (str1 == NULL)
		return 0;
	if (str2 == NULL)
		return 0;

	size_t Result = 0;

	size_t Strlen1 = strlen(str1);
	size_t Strlen2 = strlen(str2);

	char *str1cpy = malloc(Strlen1+1);
	char *str2cpy = malloc(Strlen2+1);

	strcpy(str1cpy, str1);
	strcpy(str2cpy, str2);

	// +1 for null terminator
	size_t RequiredBytes = Strlen1 + Strlen2 + 1;

	char *Buffer;

	if (buffer != NULL) {
		Buffer = *buffer;
		if (BufferSize < RequiredBytes) {
			Buffer = realloc(Buffer, RequiredBytes);
			if (Buffer == NULL) goto done;
			*buffer = Buffer;
		}
	} else {
		Buffer = malloc(RequiredBytes);
		if (Buffer == NULL) goto done;
	}

//	printf("copy - %s\n", str1cpy);
	strcpy(Buffer, str1cpy);
//	printf("concat - %s\n", str2cpy);
	strcat(Buffer, str2cpy);

	Result = Strlen1+Strlen2;

done:
	free(str1cpy);
	free(str2cpy);

	return Result;
}

char *ProgressBar(uint8_t Percentage, uint64_t Precision, Color_t ProgressBarColor) {
	char *str = malloc(1);
	str[0] = 0;
	size_t len = 0;

	len = AppendStrings(str, "[", &str, len);

	uint64_t ProgressPerCell = 100 / Precision;

	size_t CharsTyped = 0;
	uint8_t Progress = 0;
	while (Progress < 100) {
		if (Progress < Percentage)
			len = AppendStrings(str, OCCUPIED_CHAR, &str, len);
		else
			len = AppendStrings(str, EMPTY_CHAR, &str, len);

		Progress += ProgressPerCell;
	}

	len = AppendStrings(str, "]", &str, len);
	return str;
}
