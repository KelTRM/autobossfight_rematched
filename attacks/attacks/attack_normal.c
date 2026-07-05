#include<stdlib.h>
#include<ui.h>
#include"../attacks.h"
#include"../rng.h"

// easy-to-use attack parameters
#define ATTACK_MINIMUM_ENERGY		5
#define ATTACK_FIRST_AVAILABLE_ROUND	0
#define ATTACK_NAME			"Normal Attack"

// Used to identify this attack. Non-unique values may result in undefined behavior
#define ATTACK_ID			1

extern uint64_t Round;

static AttackData_t DoAttack(Attack_t *Self, Entity_t *Target, Entity_t *Attacker);

const Attack_t NormalAttack = {
	.AttackName=ATTACK_NAME,

	.MinimumEnergy=ATTACK_MINIMUM_ENERGY,
	.FirstAvailableRound=ATTACK_FIRST_AVAILABLE_ROUND,

	.Available=DefaultCanAttack,
	.Attack=DoAttack,

	.AppliesToAllies=0,
	.AppliesToEnemies=1,

	.ID=ATTACK_ID
};

static AttackData_t DoAttack(Attack_t *Self, Entity_t *Target, Entity_t *Attacker) {
	if (Self->Available(Self, Attacker) == 0)
		return NothingAttack.Attack(Self, Target, Attacker);

	RemoveEnergy(Attacker, ATTACK_MINIMUM_ENERGY);

	Health_t Damage = GetRandomIntBetween(0, Attacker->Attack);

	AttackData_t *AttackMissed = MissedAttack(Self, Target, Attacker);
	if (AttackMissed != NULL) {
		AttackData_t r = *AttackMissed;
		free(AttackMissed);
		return r;
	}

	Health_t PriorHealth = Target->HealthPoints;
	Damage = DamageEntity(Attacker, Target, Damage);

	AttackData_t Result;
	GenAttackData(Attacker, Target, Self, Damage, PriorHealth, &Result);
	
	return Result;
}

