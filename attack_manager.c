#include"attack_manager.h"
#include<stddef.h>
#include<stdlib.h>
#include<string.h>

// How many attacks can be registered at once
#define ATTACK_LIMIT	256

Attack_t **Attacks = NULL;
size_t AttackCount = 0;
size_t AttacksSize = 0;

int InitAttackRegistrar() {
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
	
	// Check if the attack count would exceed the maximum
	if (AttackCount < ATTACK_LIMIT)
		AttackCount++;
	else return 0;

	Attacks[AttackCount-1] = Attack;
	return 1;
}

Attack_t *GetAttackAtIndex(size_t Index) {
	if (Index >= AttackCount) return NULL;
	return Attacks[Index];
}
