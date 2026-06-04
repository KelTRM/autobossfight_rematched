#pragma once

#include"entity.h"

typedef struct AttackData {
	Entity_t *Target;
	Entity_t *Attacker;

	Health_t Attack;
	Health_t Damage;
	
	Health_t PriorHealth;
} AttackData_t;

typedef int (*CanAttack)(Entity_t *Attacker);
typedef AttackData_t (*OnAttack)(Entity_t *Target, Entity_t *Attacker);

typedef struct Attack {
	const char *AttackName;

	Energy_t MinimumEnergy;
	uint64_t FirstAvailableRound;

	CanAttack Available;
	OnAttack Attack;
} Attack_t;
