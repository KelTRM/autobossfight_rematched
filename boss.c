#include"boss.h"
#include"attacks/attacks.h"
#include"attack_manager.h"
#include "entity.h"
#include"rng.h"

int BossTurn(Entity_t *CurrentPlayer, uint64_t Round);

Entity_t CreateBoss(const char *Name, Health_t HP) {
	Entity_t Boss;
	Boss.Name = Name;
	Boss.HealthPoints = HP;
	Boss.Energy = 100;

	Boss.EnemyCount = 0;
	Boss.Enemies = NULL;

	Boss.HealingMinimum = 0;
	Boss.HealingMaximum = 0;

	Boss.Attack = 10000;

	Boss.TakeTurn = BossTurn;
	Boss.AI_RechargeEnergy = 0;

	return Boss;
}

#define BOSS_ENERGY_RECHARGE_THRESHOLD	90
#define BOSS_ENERGY_RECHARGE_TO		95

const Attack_t *BossAI(Entity_t *Boss) {
	if (Boss->Energy < BOSS_ENERGY_RECHARGE_THRESHOLD)
		Boss->AI_RechargeEnergy = BOSS_ENERGY_RECHARGE_TO;

	if (Boss->Energy < Boss->AI_RechargeEnergy) {
		return &NothingAttack;
	}

	return &NormalAttack;
}

int BossTurn(Entity_t *CurrentPlayer, uint64_t Round) {
	const Attack_t *ChosenAttack = BossAI(CurrentPlayer);

	int EntityToAttack = GetRandomIntBetween(0, CurrentPlayer->EnemyCount);
	AttackEntity((Attack_t*)ChosenAttack, GetEnemyAtIndex(CurrentPlayer, EntityToAttack), CurrentPlayer);
	return 0;
}
