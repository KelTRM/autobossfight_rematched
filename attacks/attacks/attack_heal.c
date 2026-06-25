#include<ui.h>
#include"../attacks.h"
#include"../rng.h"

// easy-to-use attack parameters
#define ATTACK_MINIMUM_ENERGY		10
#define ATTACK_FIRST_AVAILABLE_ROUND	0
#define ATTACK_NAME			"Heal"

// Used to identify this attack. Non-unique values may result in undefined behavior
#define ATTACK_ID			3

extern uint64_t Round;

static int CanDoAttack(Entity_t *Attacker);
static AttackData_t DoAttack(Attack_t *Self, Entity_t *Target, Entity_t *Attacker);

static void Announcer(AttackData_t *Attack);

const Attack_t HealAttack = {
	.AttackName=ATTACK_NAME,

	.MinimumEnergy=ATTACK_MINIMUM_ENERGY,
	.FirstAvailableRound=ATTACK_FIRST_AVAILABLE_ROUND,

	.Available=CanDoAttack,
	.Attack=DoAttack,

	.Announcer=Announcer,

	.AppliesToAllies=1,
	.AppliesToEnemies=0,

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

	Health_t HealingAmount = GetRandomIntBetween(Attacker->HealingMinimum, Attacker->HealingMaximum);

	Health_t PriorHealth = Target->HealthPoints;
	HealingAmount = HealEntity(Target, HealingAmount, 0);

	AttackData_t Result;
	Result.Attacker = Attacker;
	Result.Target = Target;
	Result.Attack = HealAttack.ID;
	Result.Damage = HealingAmount;
	Result.PriorHealth = PriorHealth;

	return Result;
}

static void Announcer(AttackData_t *Attack) {
	printf("%s healed %s by %llu health.\n",
			Attack->Attacker->Name, Attack->Target->Name, Attack->Damage);
}
