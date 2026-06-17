#pragma once

#include<stdint.h>
#include<stddef.h>

#define MAX_ENERGY			100		// Maximum energy an entity can possess
#define ENERGY_GAIN_PER_ROUND		3		// The amount of energy an entity may gain in a round.

struct Entity;
typedef struct Entity Entity_t;

typedef uint64_t Energy_t;
typedef uint64_t Health_t;

typedef int (*EntityTurnHandler)(Entity_t *Self, uint64_t CurrentRound);

#include"player.h"
#include"boss.h"

struct Entity {
	const char *Name;
	Health_t HealthPoints;

	Health_t Attack;
	//uint64_t EntityID;

	// Healing amounts. A heal may provide between HealingMinimum to HealingMaximum health to this entity
	Health_t HealingMinimum;
	Health_t HealingMaximum;

	Energy_t Energy;

	struct Entity **Enemies;
	size_t EnemyCount;
	EntityTurnHandler TakeTurn;

	int AI_RechargeEnergy;
};

Health_t DamageEntity(Entity_t *Target, Health_t Damage);
Energy_t RemoveEnergy(Entity_t *Target, Energy_t Draw);
Energy_t EnergizeEntity(Entity_t *Target, Energy_t Energy);

size_t AddEntityEnemy(Entity_t *To, Entity_t *Enemy);
Entity_t *GetEnemyAtIndex(Entity_t *From, size_t Index);
