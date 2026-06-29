#include<stdlib.h>
#include<stddef.h>
#include<string.h>
#include<math.h>
#include "console_manager/console.h"
#include"ui.h"
#include"../entity.h"
#include"color/color.h"
#include"strings.h"

// for simplicity sake
//#define printf(...)	bprintf(INVALID_BUFFER_HANDLE, __VA_ARGS__)

#define BOX_CHAR		"▅"
#define ENERGY_DISP_PRECISION	20

extern Entity_t *Entities;
extern size_t EntityCount;
//extern Entity_t Players[3];

int RefreshScoreboard(int PreserveCursorPosition) {
	if (PreserveCursorPosition)
		printf("\x1b[s");
	printf("\x1b[H");

	size_t MaximumNameLength = GetLongestElementString(Entities, Name, EntityCount);
	Health_t MaximumHealth = 0;

	size_t MaximumFormNameLength = 0;

	for (size_t i = 0; i < EntityCount; i++) {
		Entity_t *CurrentEntity = Entities + i;

		Transformation_t *EntityForm = CurrentEntity->EntityTransformation;
		size_t EntityFormNameLength = strlen(EntityForm->Name);

		MaximumFormNameLength = max(EntityFormNameLength, MaximumFormNameLength);

		if (CurrentEntity->HealthPoints > MaximumHealth)
			MaximumHealth = CurrentEntity->HealthPoints;
	}

	size_t MaximumHealthDigits = CalculateDigitsInNumber(MaximumHealth);
	//printf("MaximumHealthDigits = %zu\n", MaximumHealthDigits);

	//EntityCategory_t EntityType = ENTITY_TYPE_NONE;

	for (size_t i = 0; i < EntityCount; i++) {
		if (Entities[i].DisplayEntity != NULL) {
			Entities[i].DisplayEntity(
					INVALID_BUFFER_HANDLE,
					&Entities[i], 1,
					MaximumHealthDigits,
					MaximumNameLength,
					MaximumFormNameLength
			);
			continue;
		}

		printf("Entity of name %s has no known renderer.", Entities[i].Name);
	}
	return 0;
}
