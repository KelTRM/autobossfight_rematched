#include"entity.h"

Health_t DamageEntity(Entity_t *Target, Health_t Damage) {
	Health_t DamageDealt = Damage;

	if (Target->HealthPoints <= Damage) {
		DamageDealt = Target->HealthPoints;
	}

	Target->HealthPoints -= DamageDealt;
	return DamageDealt;
}

Energy_t RemoveEnergy(Entity_t *Target, Energy_t Draw) {
	if (Target->Energy < Draw)
		Draw = Target->Energy;
	Target->Energy -= Draw;
	return Draw;
}
