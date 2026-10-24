#pragma once

#include"../entity.h"

struct PlayerDefinition {
	const char *Name;		// name of the player
	const Health_t HP;		// health of the player
	const Health_t Heal_Min;	// minimum amount the player may heal another by
	const Health_t Heal_Max;	// maximum amount the player may heal another by
	
	const uint32_t Color;
};

struct BossDefinition {
	const char *Name;
	const Health_t HP;
	// no healing because boss bad
};
