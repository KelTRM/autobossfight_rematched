#pragma once

#include<stdint.h>

typedef uint64_t Energy_t;
typedef uint64_t Health_t;

struct Entity {
	const char *Name;
	Health_t HealthPoints;

	Health_t Attack;
	//uint64_t EntityID;

	// Healing amounts. A heal may provide between HealingMinimum to HealingMaximum health to this entity
	Health_t HealingMinimum;
	Health_t HealingMaximum;

	Energy_t Energy;
};

typedef struct Entity Entity_t;
