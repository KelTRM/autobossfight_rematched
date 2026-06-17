#include"boss.h"
#include "attack.h"
#include"attack_manager.h"

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

	return Boss;
}

int BossTurn(Entity_t *CurrentPlayer, uint64_t Round) {
	Attack_t *ChosenAttack = GetAttackAtIndex(1);
	AttackEntity(ChosenAttack, CurrentPlayer, CurrentPlayer);
}
