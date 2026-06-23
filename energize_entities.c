#include"entity.h"

extern Entity_t *Entities;
extern size_t EntityCount;

void EnergizeEntities(void) {
	for (size_t i = 0; i < EntityCount; i++) {
		Entity_t *e = &Entities[i];

		if (IsEntityAlive(e))
			EnergizeEntity(e, ENERGY_GAIN_PER_ROUND);
	}
}
