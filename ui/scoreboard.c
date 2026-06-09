#include<stddef.h>
#include<string.h>
#include"console_manager/console.h"
#include"../entity.h"

// for simplicity sake
#define printf(...)	bprintf(INVALID_BUFFER_HANDLE, __VA_ARGS__)

Entity_t Players[3];

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


}
