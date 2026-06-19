#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include<string.h>
#include<math.h>
#include"ui.h"
#include"../entity.h"
#include"color/color.h"
#include"strings.h"

// for simplicity sake
//#define printf(...)	bprintf(INVALID_BUFFER_HANDLE, __VA_ARGS__)

#define BOX_CHAR		"▅"
#define ENERGY_DISP_PRECISION	20

#define max(x, y)	((x)>(y)?(x):(y))

extern Entity_t *Entities;
extern size_t EntityCount;
//extern Entity_t Players[3];

int RefreshScoreboard(int PreserveCursorPosition) {
	if (PreserveCursorPosition)
		printf("\x1b[s");
	printf("\x1b[H");

	size_t MaximumNameLength = 0;
	Health_t MaximumHealth = 0;

	for (size_t i = 0; i < EntityCount; i++) {
		Entity_t *t = &Entities[i];

//		printf("i = %p\n", t->Name);
		const char *Name = t->Name;
		size_t NameLength = strlen(Name);

		if (NameLength > MaximumNameLength)
			MaximumNameLength = NameLength;

		if (t->HealthPoints > MaximumHealth)
			MaximumHealth = t->HealthPoints;
	}

	size_t MaximumHealthDigits = CalculateDigitsInNumber(MaximumHealth);
	//printf("MaximumHealthDigits = %zu\n", MaximumHealthDigits);

	EntityCategory_t EntityType = ENTITY_TYPE_NONE;

	for (size_t i = 0; i < EntityCount; i++) {
		if (Entities[i].DisplayEntity != NULL) {
			Entities[i].DisplayEntity(
					&Entities[i],
					MaximumHealthDigits,
					MaximumNameLength
			);
			continue;
		}

		if (EntityType != Entities[i].Type && EntityType != ENTITY_TYPE_NONE) {
			printf("\n");
		}
		EntityType = Entities[i].Type;

		//printf("\x1b[2K");

		char *NameStr = PadRight(Entities[i].Name, MaximumNameLength, ' ');
		char *HpStr = IntToStr(Entities[i].HealthPoints);

		char *HealthStr = PadLeft(HpStr, MaximumHealthDigits, ' ');
		free(HpStr);
		
		printf("%s has %s hp [", NameStr, HealthStr);
		
		GetTerminalForegroundColorStr(0, 100, 255);
		for (int j = 0; j < MAX_ENERGY; j += MAX_ENERGY / ENERGY_DISP_PRECISION) {
			if (Entities[i].Energy > j)	printf("%s", BOX_CHAR);
			else				printf(" ");
		}
		ResetTerminalForegroundColorStr();
		printf("] (%d%%)\n", Entities[i].Energy);

		free(NameStr);
		free(HealthStr);
	}
	return 0;
}
