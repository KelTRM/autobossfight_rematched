#pragma once

#include"../entity.h"

typedef struct Attack Attack_t;

typedef uint64_t AttackID_t;

typedef struct AttackData {
	Entity_t *Target;
	Entity_t *Attacker;

	AttackID_t Attack;
	Health_t Damage;
	
	Health_t PriorHealth;
} AttackData_t;

typedef int (*CanAttack)(Entity_t *Attacker);
typedef AttackData_t (*OnAttack)(Attack_t *Self, Entity_t *Target, Entity_t *Attacker);
typedef void (*AttackAnnouncer)(AttackData_t *Attack);

struct Attack {
	const char *AttackName;

	Energy_t MinimumEnergy;
	uint64_t FirstAvailableRound;

	CanAttack Available;
	OnAttack Attack;

	AttackAnnouncer Announcer;

	int AppliesToAllies;
	int AppliesToEnemies;

	AttackID_t ID;
};

AttackData_t AttackEntity(Attack_t *Attack, Entity_t *Target, Entity_t *Attacker);
