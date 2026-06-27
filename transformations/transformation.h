#pragma once

#include"../entity.h"

typedef struct Transformation Transformation_t;

typedef Health_t (*GetTransformedAttack_t)(Transformation_t *Self, Entity_t *From, Health_t BaseDamage);

struct Transformation {
	char *Name;
	
	Energy_t Cost;
	Energy_t Upkeep;

	uint64_t TransformationMultiplier;

	GetTransformedAttack_t GetDamage;
};

