#pragma once

#include<stdint.h>
#include<stddef.h>

#include"ui/color/color.h"

#define MAX_ENERGY			100		// Maximum energy an entity can possess
#define ENERGY_GAIN_PER_ROUND		3		// The amount of energy an entity may gain in a round.

struct Entity;
typedef struct Entity Entity_t;

typedef uint64_t Energy_t;
typedef uint64_t Health_t;

typedef int (*EntityTurnHandler_t)(Entity_t *Self, uint64_t CurrentRound);
typedef int (*EntityDisplayHandler_t)(Entity_t *Self, int ShowEnergy, size_t HealthPadding, size_t NamePadding);

#include"player.h"
#include"boss.h"

typedef enum EntityCategory {
	ENTITY_TYPE_PLAYER,
	ENTITY_TYPE_BOSS,
	ENTITY_TYPE_NONE
} EntityCategory_t;

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
	EntityCategory_t Type;

	EntityTurnHandler_t TakeTurn;
	EntityDisplayHandler_t DisplayEntity;

	Color_t EntityColor;

	int AI_RechargeEnergy;
};

Health_t DamageEntity(Entity_t *Target, Health_t Damage);
Health_t HealEntity(Entity_t *Target, Health_t Health);
Energy_t RemoveEnergy(Entity_t *Target, Energy_t Draw);
Energy_t EnergizeEntity(Entity_t *Target, Energy_t Energy);

size_t AddEntityEnemy(Entity_t *To, Entity_t *Enemy);
Entity_t *GetEnemyAtIndex(Entity_t *From, size_t Index);

char *GetEntityNameStr(Entity_t *Entity);
