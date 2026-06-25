#include<stdlib.h>
#include"attack.h"
#include<strings.h>
#include"../rng.h"

void DefaultAnnouncer(AttackData_t *Attack) {
	char *str = IntToStr(Attack->Damage);
	printf("%s dealt %s damage to %s\n",
			Attack->Attacker->Name, str, Attack->Target->Name);
	free(str);
}

// NULL if attack hit. &NothingAttack if missed
AttackData_t *MissedAttack(Attack_t *Self, Entity_t *Target, Entity_t *Attacker) {
	if (GetRandomIntBetween(1, 5) < 2) {
		printf("%s has missed their attack on %s.\n", Attacker->Name, Target->Name);
		
		AttackData_t *r = malloc(sizeof(AttackData_t));

		// if allocation fails. just pretend it hit. not the end of the world
		if (r == NULL)
			return NULL;

		r->Attack = Self->ID;
		r->Attacker = Attacker;
		r->Target = Target;
		r->Damage = 0;
		r->PriorHealth = Target->HealthPoints;

		return r;
	}

	return NULL;
}
