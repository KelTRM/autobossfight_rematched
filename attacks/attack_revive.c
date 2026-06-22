#include"../ui/ui.h"
#include"attacks.h"
#include"../rng.h"

// easy-to-use attack parameters
#define ATTACK_MINIMUM_ENERGY		40
#define ATTACK_FIRST_AVAILABLE_ROUND	0
#define ATTACK_NAME			"Revive"

// Used to identify this attack. Non-unique values may result in undefined behavior
#define ATTACK_ID			6

extern int Round;

static int CanDoAttack(Entity_t *Attacker);
static AttackData_t DoAttack(Entity_t *Target, Entity_t *Attacker);

static void Announcer(AttackData_t *Attack);

const Attack_t ReviveAttack = {
	.AttackName=ATTACK_NAME,

	.MinimumEnergy=ATTACK_MINIMUM_ENERGY,
	.FirstAvailableRound=ATTACK_FIRST_AVAILABLE_ROUND,

	.Available=CanDoAttack,
	.Attack=DoAttack,

	.Announcer=Announcer,

	.AppliesToAllies=1,
	.AppliesToEnemies=0
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

	//Health_t Damage = GetRandomIntBetween(0, Attacker->Attack);

	//if (GetRandomIntBetween(1, 5) < 2) {
	//	printf("%s has missed their attack on %s.\n", Attacker->Name, Target->Name);
	//	return NothingAttack.Attack(Target, Attacker);
	//}
	
	if (IsEntityAlive(Target))
		return NothingAttack.Attack(Target, Attacker);

	Health_t HealingAmount = GetRandomIntBetween(0, Attacker->HealthPoints);
	Health_t HealthDrained = (HealingAmount / GetRandomIntBetween(3, 6)) * 2;

	Health_t PriorHealth = Target->HealthPoints;

	HealingAmount = HealEntity(Target, HealingAmount);
	HealthDrained = DamageEntity(Attacker, Attacker, HealthDrained);

	AttackData_t Result;
	Result.Attacker = Attacker;
	Result.Target = Target;
	Result.Attack = ATTACK_ID;
	Result.Damage = HealingAmount;
	Result.PriorHealth = PriorHealth;

	return Result;
}

static void Announcer(AttackData_t *Attack) {
	printf("%s has revived %s, giving him %llu hp.\n",
			Attack->Attacker->Name, Attack->Target->Name, Attack->Damage);
}
