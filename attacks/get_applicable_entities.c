#include<stdlib.h>
#include"../ui/ui.h"
#include"attacks.h"

extern Entity_t *Entities;
extern size_t EntityCount;

// genuinely peak names
int IsEnemy(Entity_t *Of, Entity_t *Whom) {
	for (size_t i = 0; i < Of->EnemyCount; i++) {
		if (Of->Enemies[i] == Whom) return 1;
	}

	return 0;
}

Entity_t **GetApplicableEntities(Attack_t *AttemptingAttack, Entity_t *Attacker, size_t *ApplicableCount) {
	if (ApplicableCount == NULL)
		return NULL;

	*ApplicableCount = 0;
	Entity_t **ApplicableEntities = calloc(1, sizeof(*Entities));

	for (size_t i = 0; i < EntityCount; i++) {
		Entity_t *CurrentOption = &Entities[i];
		int OptionIsEnemy = IsEnemy(Attacker, CurrentOption);

		if ((AttemptingAttack->AppliesToEnemies && OptionIsEnemy) ||
		    (AttemptingAttack->AppliesToAllies && !OptionIsEnemy)) {
			(*ApplicableCount)++;
			Entity_t **NewPtr = realloc(ApplicableEntities, *ApplicableCount * sizeof(Entity_t*));
			if (NewPtr == NULL)
				return ApplicableEntities;

			NewPtr[*ApplicableCount - 1] = CurrentOption;
			ApplicableEntities = NewPtr;
		}
	}

	return ApplicableEntities;
}
