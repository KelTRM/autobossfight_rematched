#include<ui.h>
#include"transformation.h"

Energy_t DefaultMaintainTransformation(Transformation_t *Self, Entity_t *Possessor) {
	if (Possessor->Energy < Self->Upkeep) {
		printf("%s has insufficient energy to upkeep their transformation.\n",
				Possessor->Name);

		Possessor->EntityTransformation = (Transformation_t*)DefaultTransformation;
		return 0;
	}

	Possessor->Energy -= Self->Upkeep;
	return Self->Upkeep;
}

Health_t DefaultGetDamage(Transformation_t *Self, Entity_t *From, Health_t BaseDamage) {
	(void)From; // to remove warning
	Health_t NewDamage = BaseDamage;
	NewDamage *= Self->TransformationMultiplier;
	NewDamage /= ATTACK_MULTIPLIER_BASE;

	return NewDamage;
}
