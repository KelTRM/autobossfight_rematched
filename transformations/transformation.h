#pragma once

typedef struct Transformation Transformation_t;

#include"../entity.h"

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

extern const Transformation_t BaseTransformation;
extern Transformation_t BossTransformation;

