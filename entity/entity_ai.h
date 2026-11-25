#pragma once

#include"attack.h"
#include"entity.h"

const Attack_t *EntityAI(Entity_t *Boss);
int AITurn(Entity_t *CurrentPlayer, uint64_t Round);
