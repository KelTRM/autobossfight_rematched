#include<stddef.h>

size_t UTF8_Strlen(const char *str) {
	if (str == NULL)
		return 0;

	size_t n = 0;

	while (*str != 0) {
		if (*(unsigned char*)str < 0x80) {
			n++;
		}
		str++;
	}

	return n;
}
