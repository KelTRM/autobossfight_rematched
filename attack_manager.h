#pragma once

#include"attack.h"
#include<stddef.h>

int InitAttackRegistrar();
int RegisterAttack(Attack_t *Attack);
Attack_t *GetAttackAtIndex(size_t Index);
