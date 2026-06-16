#pragma once

#include<stdint.h>

#define MAX_ENERGY			100		// Maximum energy an entity can possess
#define ENERGY_GAIN_PER_ROUND		3		// The amount of energy an entity may gain in a round.

typedef uint64_t Energy_t;
typedef uint64_t Health_t;

typedef struct Entity {
	const char *Name;
	Health_t HealthPoints;

	Health_t Attack;
	//uint64_t EntityID;

	// Healing amounts. A heal may provide between HealingMinimum to HealingMaximum health to this entity
	Health_t HealingMinimum;
	Health_t HealingMaximum;

	Energy_t Energy;
} Entity_t;

Health_t DamageEntity(Entity_t *Target, Health_t Damage);
Energy_t RemoveEnergy(Entity_t *Target, Energy_t Draw);
Energy_t EnergizeEntity(Entity_t *Target, Energy_t Energy);
