#include"../transformation.h"

#define ToFixed(x)			((uint64_t)((x) * ATTACK_MULTIPLIER_BASE))

#define TRANSFORMATION_ID		2
#define TRANSFORMATION_NAME		"Super"
#define TRANSFORMATION_DESCRIPTION	"Does 3x damage"

#define TRANSFORMATION_COST		20
#define TRANSFORMATION_UPKEEP		5

#define TRANSFORMATION_MULTIPLIER	ToFixed(2)

static Health_t GetDamage(Transformation_t *Self, Entity_t *From, Health_t BaseDamage);

const Transformation_t SuperTransformation = {
	.Name=TRANSFORMATION_NAME,
	.FormDescription=TRANSFORMATION_DESCRIPTION,

	.Cost=TRANSFORMATION_COST,
	.Upkeep=TRANSFORMATION_UPKEEP,
	.TransformationMultiplier=TRANSFORMATION_MULTIPLIER,

	.GetDamage=GetDamage,
	.MaintainTransformation=MaintainTransformation,

	.ID=TRANSFORMATION_ID
};

static Health_t GetDamage(Transformation_t *Self, Entity_t *From, Health_t BaseDamage) {
	(void)From; // to remove warning
	Health_t NewDamage = BaseDamage;
	NewDamage *= Self->TransformationMultiplier;
	NewDamage /= ATTACK_MULTIPLIER_BASE;

	return NewDamage;
}
