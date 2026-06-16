#pragma once

#include"entity.h"

typedef uint64_t AttackID_t;

typedef struct AttackData {
	Entity_t *Target;
	Entity_t *Attacker;

	AttackID_t Attack;
	Health_t Damage;
	
	Health_t PriorHealth;
} AttackData_t;

typedef int (*CanAttack)(Entity_t *Attacker);
typedef AttackData_t (*OnAttack)(Entity_t *Target, Entity_t *Attacker);
typedef void (*AttackAnnouncer)(AttackData_t *Attack);

typedef struct Attack {
	const char *AttackName;

	Energy_t MinimumEnergy;
	uint64_t FirstAvailableRound;

	CanAttack Available;
	OnAttack Attack;

	AttackAnnouncer Announcer;
} Attack_t;

void DoAttack(Attack_t *Attack, Entity_t *Target, Entity_t *Attacker);
