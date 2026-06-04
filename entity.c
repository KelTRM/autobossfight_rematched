#include"entity.h"

Health_t DamageEntity(Entity_t *Target, Health_t Damage) {
	Health_t DamageDealt = Damage;

	if (Target->HealthPoints <= Damage) {
		DamageDealt = Target->HealthPoints;
	}

	Target->HealthPoints -= DamageDealt;
	return DamageDealt;
}
