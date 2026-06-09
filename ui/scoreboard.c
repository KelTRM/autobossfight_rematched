#include<stdlib.h>
#include<stddef.h>
#include<string.h>
#include"console_manager/console.h"
#include"../entity.h"

// for simplicity sake
#define printf(...)	bprintf(INVALID_BUFFER_HANDLE, __VA_ARGS__)

#define max(x, y)	((x)>(y)?(x):(y))

Entity_t Players[3];

char *PadRight(char *str, size_t n, char ch) {
	size_t Length = strlen(str);
	size_t NewLength = max(n, Length);
	// add 1 for null terminator
	char *NewStr = malloc(NewLength+1);
	memset(NewStr, Length, ch);
	memcpy(NewStr, str, Length);

	NewStr[NewLength] = '\0';
}

int RefreshScoreboard(int PreserveCursorPosition) {
	if (PreserveCursorPosition)
		printf("\x1b[s");
	printf("\x1b[H");

	size_t MaximumNameLength = 0;
	Health_t MaximumHealth = 0;

	for (size_t i = 0; i < sizeof(Players) / sizeof(*Players); i++) {
		Entity_t *t = &Players[i];

		const char *Name = t->Name;
		size_t NameLength = strlen(Name);

		if (NameLength > MaximumNameLength)
			MaximumNameLength = NameLength;

		if (t->HealthPoints > MaximumHealth)
			MaximumHealth = t->HealthPoints;
	}

	for (size_t i = 0; i < sizeof(Players) / sizeof(*Players); i++) {
		printf("\x1b[2K");

		char *Name = malloc(MaximumNameLength+1);
		memset(Name, ' ', MaximumNameLength);
		Name[MaximumNameLength] = '\0';

		memcpy(Name, Players[i].Name, strlen(Players[i].Name));
	}
}
