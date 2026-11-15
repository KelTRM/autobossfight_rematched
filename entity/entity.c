#include <stdio.h>
#include<stdlib.h>
#include"entity.h"

Health_t DamageEntity(Entity_t *From, Entity_t *Target, Health_t Damage) {
	Health_t DamageDealt;// = (Damage * From->AttackMultiplier) / ATTACK_MULTIPLIER_BASE;

	DamageDealt = From->EntityTransformation->GetDamage(From->EntityTransformation, From, Damage);
//	From->EntityTransformation->TransformationMultiplier(From, Target, Damage);

	if (Target->HealthPoints <= DamageDealt) {
		DamageDealt = Target->HealthPoints;
	}

	Target->HealthPoints -= DamageDealt;
	return DamageDealt;
}

Health_t HealEntity(Entity_t *Target, Health_t Health, int PermitNecromancy) {
	if (!IsEntityAlive(Target) && !PermitNecromancy)
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

char *GetEntityNameStr(Entity_t *Entity) {
	char *EntityColor = GetColor(38, Entity->EntityColor);
	char *ClearColor = GetClearColorStr();

	const char *Format = "%s%s%s";

	size_t Length = snprintf(NULL, 0, Format, EntityColor, Entity->Name, ClearColor);

	char *str = malloc(Length+1);
	snprintf(str, Length+1, Format, EntityColor, Entity->Name, ClearColor);

	free(EntityColor);

	return str;
}

int IsEntityAlive(Entity_t *Entity) {
	if (Entity->HealthPoints > 0) {
		return 1;
	}

	return 0;
}

int HasLivingEnemies(Entity_t *Entity) {
	for (size_t i = 0; i < Entity->EnemyCount; i++) {
		Entity_t *Enemy = GetEnemyAtIndex(Entity, i);
		if (Enemy == NULL) continue;

		if (IsEntityAlive(Enemy))
			return 1;
	}

	return 0;
}
