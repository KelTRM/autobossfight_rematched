#pragma once

#include"../entity.h"

typedef struct Transformation Transformation_t;

typedef Health_t (*GetTransformedAttack_t)(Transformation_t *Self, Entity_t *From, Health_t BaseDamage);
typedef Energy_t (*Maintain_t)(Transformation_t *Self, Entity_t *Possessor);

struct Transformation {
	char *Name;

	Energy_t Cost;
	Energy_t Upkeep;

	uint64_t TransformationMultiplier;

	GetTransformedAttack_t GetDamage;
	Maintain_t MaintainTransformation;
};

Energy_t MaintainTransformation(Transformation_t *Self, Entity_t *Possessor);

