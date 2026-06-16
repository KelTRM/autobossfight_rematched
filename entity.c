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

Energy_t EnergizeEntity(Entity_t *Target, Energy_t Energy) {
	if (Target->Energy + Energy > 100)
		Energy = 100 - Target->Energy;
	Target->Energy += Energy;
	return Energy;
}
