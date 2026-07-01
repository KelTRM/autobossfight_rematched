#pragma once

#include"attack.h"

extern const Attack_t NormalAttack;
extern const Attack_t NothingAttack;
extern const Attack_t FullPowerAttack;
extern const Attack_t ComboAttack;
extern const Attack_t HealAttack;
extern const Attack_t ReviveAttack;
extern const Attack_t TransformAttack;
extern const Attack_t HalfPowerAttack;

void DefaultAnnouncer(AttackData_t *Attack);
AttackData_t *MissedAttack(Attack_t *Self, Entity_t *Target, Entity_t *Attacker);

