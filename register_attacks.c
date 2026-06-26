#include"attacks/attacks.h"
#include"attack_manager.h"
#include<stddef.h>

const Attack_t *AttacksToRegister[] = {
	&NothingAttack,
	&NormalAttack,
	&HealAttack,
	&ComboAttack,
	&ReviveAttack,
	&FullPowerAttack,
};

size_t InitAttacks(void) {
	InitAttackRegistrar();

	size_t AttackCount = sizeof(AttacksToRegister) / sizeof(*AttacksToRegister);
	size_t RegisteredAttacks = 0;

	for (size_t i = 0; i < AttackCount; i++) {
		RegisteredAttacks += RegisterAttack((Attack_t*)AttacksToRegister[i]);
	}

	BuildAttackList();

	return RegisteredAttacks;
}
