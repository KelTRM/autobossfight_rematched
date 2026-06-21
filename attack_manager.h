#pragma once

#include"attack.h"
#include<stddef.h>

int InitAttackRegistrar();
int RegisterAttack(Attack_t *Attack);
Attack_t *GetAttackAtIndex(size_t Index);
Entity_t **GetApplicableEntities(Attack_t *AttemptingAttack, Entity_t *Attacker, size_t *ApplicableCount);
