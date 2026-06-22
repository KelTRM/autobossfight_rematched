#include"../ui/ui.h"
#include"attacks.h"
#include"../rng.h"

// easy-to-use attack parameters
#define ATTACK_MINIMUM_ENERGY		90
#define ATTACK_FIRST_AVAILABLE_ROUND	0
#define ATTACK_NAME			"Full power attack"

// Used to identify this attack. Non-unique values may result in undefined behavior
#define ATTACK_ID			3

extern int Round;

static int CanDoAttack(Entity_t *Attacker);
static AttackData_t DoAttack(Entity_t *Target, Entity_t *Attacker);

const Attack_t FullPowerAttack = {
	.AttackName=ATTACK_NAME,

	.MinimumEnergy=ATTACK_MINIMUM_ENERGY,
	.FirstAvailableRound=ATTACK_FIRST_AVAILABLE_ROUND,

	.Available=CanDoAttack,
	.Attack=DoAttack,

	.Announcer=DefaultAnnouncer,

	.AppliesToAllies=0,
	.AppliesToEnemies=1
};

static int CanDoAttack(Entity_t *Attacker) {
	if (Attacker->Energy < NormalAttack.MinimumEnergy)
		return 0;

	if (Round < NormalAttack.FirstAvailableRound)
		return 0;

	return 1;
}

static AttackData_t DoAttack(Entity_t *Target, Entity_t *Attacker) {
	if (CanDoAttack(Attacker) == 0)
		return NothingAttack.Attack(Target, Attacker);

	RemoveEnergy(Attacker, ATTACK_MINIMUM_ENERGY);

	Health_t Damage = 0;
	for (int i = 0; i < 9; i++) {
		Damage += GetRandomIntBetween(0, Attacker->Attack);
	}

	if (GetRandomIntBetween(1, 5) < 2) {
		printf("%s has missed their attack on %s.\n", Attacker->Name, Target->Name);
		return NothingAttack.Attack(Target, Attacker);
	}

	Health_t PriorHealth = Target->HealthPoints;
	Damage = DamageEntity(Attacker, Target, Damage);

	AttackData_t Result;
	Result.Attacker = Attacker;
	Result.Target = Target;
	Result.Attack = ATTACK_ID;
	Result.Damage = Damage;
	Result.PriorHealth = PriorHealth;

	return Result;
}


