#pragma once

#include <cstdint>

typedef uint64_t Energy_t;
typedef uint64_t Health_t;

struct Entity {
	char *Name;
	Health_t HealthPoints;

	const Health_t Attack;
	//uint64_t EntityID;

	//Health_t HealingMinimum;
	//Health_t HealingMaximum;

	//Energy_t Energy;
};
