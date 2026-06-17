#include<stdlib.h>
#include"entity.h"

Health_t DamageEntity(Entity_t *Target, Health_t Damage) {
	Health_t DamageDealt = Damage;

	if (Target->HealthPoints <= Damage) {
		DamageDealt = Target->HealthPoints;
	}

	Target->HealthPoints -= DamageDealt;
	return DamageDealt;
}

Health_t HealEntity(Entity_t *Target, Health_t Health) {
	if (Target->HealthPoints == 0)
		return 0;

	Target->HealthPoints += Health;
	return Health;
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

size_t AddEntityEnemy(Entity_t *To, Entity_t *Enemy) {
	if (Enemy == NULL)
		return To->EnemyCount;

	if (To->Enemies == NULL) {
		To->Enemies = malloc(sizeof(Entity_t*));
		To->EnemyCount = 0;

		if (To->Enemies == NULL)
			return 0;
	} else {
		Entity_t **newptr = realloc(To->Enemies, (To->EnemyCount+1) * sizeof(Entity_t*));
		if (newptr == NULL)
			return To->EnemyCount;

		To->Enemies = newptr;
	}

	To->Enemies[To->EnemyCount++] = Enemy;
	return To->EnemyCount;
}

Entity_t *GetEnemyAtIndex(Entity_t *From, size_t Index) {
	// safety first
	if (From->Enemies == NULL)
		return NULL;

	if (From->EnemyCount <= Index)
		return NULL;

	return From->Enemies[Index];
}
