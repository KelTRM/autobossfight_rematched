#pragma once

#include"entity.h"

typedef struct AttackData {

} AttackData_t;

typedef int (*CanAttack)(Entity *p);
typedef AttackData_t (*OnAttack)(Entity *Target, Entity *Attacker);

typedef struct Attack {
	const char *AttackName;

	Energy_t MinimumEnergy;
	uint64_t FirstAvailableRound;

	CanAttack Available;
	OnAttack Attack;
} Attack_t;
