#include<ui.h>
#include"transformation.h"

Energy_t MaintainTransformation(Transformation_t *Self, Entity_t *Possessor) {
	if (Possessor->Energy < Self->Upkeep) {
		printf("%s has insufficient energy to upkeep their transformation.\n",
				Possessor->Name);

		Possessor->EntityTransformation = (Transformation_t*)DefaultTransformation;
		return 0;
	}

	Possessor->Energy -= Self->Upkeep;
	return Self->Upkeep;
}
