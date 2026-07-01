#include<stdlib.h>
#include<ui.h>
#include"../attacks.h"
#include"../rng.h"

// easy-to-use attack parameters
#define ATTACK_MINIMUM_ENERGY		45
#define ATTACK_FIRST_AVAILABLE_ROUND	0
#define ATTACK_NAME			"Half power attack"

// Used to identify this attack. Non-unique values may result in undefined behavior
#define ATTACK_ID			3

extern uint64_t Round;

static int CanDoAttack(Entity_t *Attacker);
static AttackData_t DoAttack(Attack_t *Self, Entity_t *Target, Entity_t *Attacker);

const Attack_t HalfPowerAttack = {
	.AttackName=ATTACK_NAME,

	.MinimumEnergy=ATTACK_MINIMUM_ENERGY,
	.FirstAvailableRound=ATTACK_FIRST_AVAILABLE_ROUND,

	.Available=CanDoAttack,
	.Attack=DoAttack,

	.Announcer=DefaultAnnouncer,

	.AppliesToAllies=0,
	.AppliesToEnemies=1,

	.ID=ATTACK_ID
};

static int CanDoAttack(Entity_t *Attacker) {
	if (Attacker->Energy < NormalAttack.MinimumEnergy)
		return 0;

	if (Round < NormalAttack.FirstAvailableRound)
		return 0;

	return 1;
}

static AttackData_t DoAttack(Attack_t *Self, Entity_t *Target, Entity_t *Attacker) {
	if (CanDoAttack(Attacker) == 0)
		return NothingAttack.Attack(Self, Target, Attacker);

	RemoveEnergy(Attacker, ATTACK_MINIMUM_ENERGY);

	Health_t Damage = 0;
	for (int i = 0; i < 3; i++) {
		Damage += GetRandomIntBetween(0, Attacker->Attack);
	}

	AttackData_t *AttackMissed = MissedAttack(Self, Target, Attacker);
	if (AttackMissed != NULL) {
		AttackData_t r = *AttackMissed;
		free(AttackMissed);
		return r;
	}	Health_t PriorHealth = Target->HealthPoints;

	Damage = DamageEntity(Attacker, Target, Damage);

	AttackData_t Result;
	Result.Attacker = Attacker;
	Result.Target = Target;
	Result.Attack = Self->ID;
	Result.Damage = Damage;
	Result.PriorHealth = PriorHealth;

	return Result;
}

