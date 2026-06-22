#pragma once

#include"../attack.h"

extern const Attack_t NormalAttack;
extern const Attack_t NothingAttack;
extern const Attack_t FullPowerAttack;
extern const Attack_t ComboAttack;
extern const Attack_t HealAttack;
extern const Attack_t ReviveAttack;

void DefaultAnnouncer(AttackData_t *Attack);
