#include"attack_manager.h"
#include<stddef.h>
#include<stdlib.h>
#include<string.h>
#include<ui.h>

// How many attacks can be registered at once
#define ATTACK_LIMIT	256

Attack_t **Attacks = NULL;
size_t AttackCount = 0;
size_t AttacksSize = 0;

int InitAttackRegistrar(void) {
	size_t AttackBufferSize = sizeof(Attack_t*) * ATTACK_LIMIT;

	if (Attacks != NULL)
		free(Attacks);

	Attacks = malloc(AttackBufferSize);
	if (Attacks == NULL) {
		return 0;
	}

	AttacksSize = AttackBufferSize;
	memset(Attacks, 0, AttackBufferSize);

	return ATTACK_LIMIT;
}

int RegisterAttack(Attack_t *Attack) {
	if (Attack == NULL) return 0;	// Don't add an attack if it's NULL

	AttackID_t ID = Attack->ID;

	// Don't add the attack if it conflicts with another of the same ID
	if (Attacks[ID] != NULL) {
		return 0;
	}

	// Check if the attack's ID is out of bounds
	if (ID >= ATTACK_LIMIT) {
		return 0;
	}

	AttackCount++;

	Attacks[ID] = Attack;
	return 1;
}

Attack_t *GetAttackAtIndex(size_t Index) {
	if (Index > ATTACK_LIMIT) return NULL;
	return Attacks[Index];
}
