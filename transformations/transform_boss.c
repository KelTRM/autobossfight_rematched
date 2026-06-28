#include"transformation.h"

#define ToFixed(x)			((uint64_t)((x) * ATTACK_MULTIPLIER_BASE))

#define TRANSFORMATION_ID		0

#define TRANSFORMATION_NAME		"Boss"
#define TRANSFORMATION_COST		0
#define TRANSFORMATION_UPKEEP		0

#define TRANSFORMATION_MULTIPLIER	ToFixed(1)

#define BOSS_MULTIPLIER_ROUND_INCREASE	ToFixed(0.2)

static Health_t GetDamage(Transformation_t *Self, Entity_t *From, Health_t BaseDamage);

Transformation_t BossTransformation = {
	.Name=TRANSFORMATION_NAME,
	.Cost=TRANSFORMATION_COST,
	.Upkeep=TRANSFORMATION_UPKEEP,
	.TransformationMultiplier=TRANSFORMATION_MULTIPLIER,

	.GetDamage=GetDamage,
	.MaintainTransformation=MaintainTransformation,
};

static Health_t GetDamage(Transformation_t *Self, Entity_t *From, Health_t BaseDamage) {
	(void)From; // to remove warning
	Health_t NewDamage = BaseDamage;
	
	NewDamage *= Self->TransformationMultiplier;
	NewDamage /= ATTACK_MULTIPLIER_BASE;

	return NewDamage;
}
