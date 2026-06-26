#include"attack_manager.h"
#include"linked_list/list.h"
#include<stddef.h>
#include<stdlib.h>
#include<string.h>
#include<ui.h>

typedef void	*AttackIter_t;

// How many attacks can be registered at once
#define ATTACK_LIMIT	256

Attack_t **Attacks = NULL;
size_t AttackCount = 0;
size_t AttacksSize = 0;

Linked_t AttackList = NULL;

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

size_t BuildAttackList(void) {
	if (AttackList != NULL)
		free(AttackList);

	size_t AttacksFound = 0;

	Linked_t *Node = &AttackList;
	for (AttackID_t ID = 0; ID < ATTACK_LIMIT; ID++) {
		if (Attacks[ID] == NULL)
			continue;

		*Node = malloc(sizeof(struct List));

		(*Node)->Value = &Attacks[ID];
		Node = &(*Node)->Next;

		AttacksFound++;
	}

	return AttacksFound;
}

AttackIter_t OpenAttackIterator(void) {
	
}

Attack_t *StepAttackIterator(AttackIter_t *Iterator) {
	(void)Iterator;
}

Attack_t *GetAttackAtIndex(size_t Index) {
	if (Index > ATTACK_LIMIT) return NULL;
	return Attacks[Index];
}
