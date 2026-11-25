#include"../transformation.h"
#include"../default_transformation.h"

#define ToFixed(x)			((uint64_t)((x) * ATTACK_MULTIPLIER_BASE))

#define TRANSFORMATION_ID		0
#define TRANSFORMATION_NAME		"Boss"
#define TRANSFORMATION_IDENTIFIER	"boss"
#define TRANSFORMATION_DESCRIPTION	"hello"

#define TRANSFORMATION_COST		0
#define TRANSFORMATION_UPKEEP		0

#define TRANSFORMATION_MULTIPLIER	ToFixed(1)

#define BOSS_MULTIPLIER_ROUND_INCREASE	ToFixed(0.2)

static Energy_t BossMaintainTransformation(Transformation_t *Self, Entity_t *Possessor);

Transformation_t BossTransformation = {
	.TransformationName=TRANSFORMATION_NAME,
	.Identifier=TRANSFORMATION_IDENTIFIER,
	.FormDescription=TRANSFORMATION_DESCRIPTION,

	.FormColor=COLOR_DARKGRAY,

	.Cost=TRANSFORMATION_COST,
	.Upkeep=TRANSFORMATION_UPKEEP,
	.TransformationMultiplier=TRANSFORMATION_MULTIPLIER,

	.GetDamage=DefaultGetDamage,
	.MaintainTransformation=BossMaintainTransformation,

	.ID=TRANSFORMATION_ID
};

static Energy_t BossMaintainTransformation(Transformation_t *Self, Entity_t *Possessor) {
	// +20% attack multiplier
	Possessor->AttackMultiplier += ATTACK_MULTIPLIER_BASE / 5;
	return TRANSFORMATION_UPKEEP;
}

//static Health_t GetDamage(Transformation_t *Self, Entity_t *From, Health_t BaseDamage) {
//	(void)From; // to remove warning
//	Health_t NewDamage = BaseDamage;
//	
//	NewDamage *= Self->TransformationMultiplier;
//	NewDamage /= ATTACK_MULTIPLIER_BASE;
//
//	return NewDamage;
///}
