#include"../attacks.h"
#include<ui.h>

// easy-to-use attack parameters
#define ATTACK_MINIMUM_ENERGY		0
#define ATTACK_FIRST_AVAILABLE_ROUND	0
#define ATTACK_NAME			"Transform"

// Used to identify this attack. Non-unique values may result in undefined behavior
#define ATTACK_ID			12

extern uint64_t Round;

Transformation_t *AskTransformation(void);

static int CanDoAttack(Entity_t *Attacker);
static AttackData_t DoAttack(Attack_t *Self, Entity_t *Target, Entity_t *Attacker);

static void Announcer(AttackData_t *Attack);

const Attack_t TransformAttack = {
	.AttackName=ATTACK_NAME,

	.MinimumEnergy=ATTACK_MINIMUM_ENERGY,
	.FirstAvailableRound=ATTACK_FIRST_AVAILABLE_ROUND,

	.Available=CanDoAttack,
	.Attack=DoAttack,

	.Announcer=Announcer,

	.AppliesToAllies=0,
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
	AttackData_t Result;
	Result.Attacker = Attacker;
	Result.Target = Target;
	Result.Attack = Self->ID;
	Result.Damage = 0;
	Result.PriorHealth = Target->HealthPoints;

	Transformation_t *Transformation = AskTransformation();

	if (Transformation->Cost > Target->Energy) {
		printf("Insufficient energy to use form [%s]\n", Transformation->Name);
		return NothingAttack.Attack(Self, Target, Attacker);
	}
	
	printf("Using transformation %s.\n", Transformation->Name);

	Target->EntityTransformation = Transformation;
	RemoveEnergy(Target, Transformation->Cost);

	if (CanDoAttack(Attacker) == 0) return Result;
	return Result;
}

static void Announcer(AttackData_t *d) {
	printf("%s transformed into %s.\n",
			d->Target->Name, d->Target->EntityTransformation->Name);
}
