#include<stdlib.h>
#include"attack.h"
#include<strings.h>
#include"../rng.h"
#include"attacks.h"
#include<assert.h>

extern uint64_t Round;

int DefaultCanAttack(Attack_t *Self, Entity_t *Attacker) {
	if (Attacker->Energy < Self->MinimumEnergy)
		return 0;

	if (Round < Self->FirstAvailableRound)
		return 0;

	return 1;
}

void DefaultAnnouncer(AttackData_t *Attack) {
	assert(Attack->Attacker != NULL);
	assert(Attack->Target != NULL);

	char *str = IntToStr(Attack->Damage);
	printf("%s dealt %s damage to %s\n",
			Attack->Attacker->Name, str, Attack->Target->Name);
	free(str);
}

// NULL if attack hit. defined if missed
AttackData_t *MissedAttack(Attack_t *Self, Entity_t *Target, Entity_t *Attacker) {
	if (GetRandomIntBetween(1, 5) < 2) {
		printf("%s has missed their attack on %s.\n", Attacker->Name, Target->Name);
		
		AttackData_t *r = malloc(sizeof(AttackData_t));

		// if allocation fails. just pretend it hit. not the end of the world
		if (r == NULL)
			return NULL;

		GenAttackData(
			Attacker,
			Target,
			Self,
			0,
			Target->HealthPoints,
			r
		);

		return r;
	}

	return NULL;
}
