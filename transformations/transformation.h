#pragma once

#include<stdint.h>

typedef uint64_t TransformationID_t;

typedef struct Transformation Transformation_t;

#include"../entity.h"

extern const Transformation_t *DefaultTransformation;

typedef Health_t (*GetTransformedAttack_t)(Transformation_t *Self, Entity_t *From, Health_t BaseDamage);
typedef Energy_t (*Maintain_t)(Transformation_t *Self, Entity_t *Possessor);

struct Transformation {
	char *Name;

	Energy_t Cost;
	Energy_t Upkeep;

	uint64_t TransformationMultiplier;

	GetTransformedAttack_t GetDamage;
	Maintain_t MaintainTransformation;

	TransformationID_t ID;
};

Energy_t MaintainTransformation(Transformation_t *Self, Entity_t *Possessor);

extern const Transformation_t BaseTransformation;
extern Transformation_t BossTransformation;
extern const Transformation_t TestTransformation;

