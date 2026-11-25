#include"../rng.h"
#include"attack.h"
#include"attacks.h"
#include"entity.h"

#define BOSS_ENERGY_RECHARGE_THRESHOLD	90
#define BOSS_ENERGY_RECHARGE_TO		95

const Attack_t *EntityAI(Entity_t *Boss) {
	if (Boss->Energy < Boss->AI_RechargeEnergy)
		return &NothingAttack;

	Boss->AI_RechargeEnergy = 0;

	if (Boss->Energy < BOSS_ENERGY_RECHARGE_THRESHOLD) {
		Boss->AI_RechargeEnergy = BOSS_ENERGY_RECHARGE_TO;
	}

	return &NormalAttack;
}

int AITurn(Entity_t *CurrentPlayer, uint64_t Round) {
	(void)Round; // get rid of unused parameter error, as it's a function pointer parameter used in another instance

	const Attack_t *ChosenAttack = EntityAI(CurrentPlayer);

	Entity_t *Target = NULL;
	while (Target == NULL) {
		int EntityToAttack = GetRandomIntBetween(0, CurrentPlayer->EnemyCount);
		Entity_t *CurrentOption = GetEnemyAtIndex(CurrentPlayer, EntityToAttack);

		if (!IsEntityAlive(CurrentOption))
			continue;
		Target = CurrentOption;
	}

	AttackEntity((Attack_t*)ChosenAttack, Target, CurrentPlayer);

	// +20% attack multiplier
	CurrentPlayer->AttackMultiplier += ATTACK_MULTIPLIER_BASE / 5;
	return 0;
}

