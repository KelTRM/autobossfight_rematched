#include"attacks/attacks.h"
#include"attack_manager.h"
#include<lua_load.h>
#include<registration.h>
#include<stddef.h>

const Attack_t *AttacksToRegister[] = {
	&NothingAttack,
	&NormalAttack,
	&HealAttack,
	&ComboAttack,
	&ReviveAttack,
	&HalfPowerAttack,
	&FullPowerAttack,
	&TransformAttack,
	&SeventyPercentPowerAttack
};

Registrar_t AttackRegistrar;

size_t InitAttacks(void *Lua) {
	InitAttackRegistrar();

	size_t AttackCount = sizeof(AttacksToRegister) / sizeof(*AttacksToRegister);
	size_t RegisteredAttacks = 0;

	for (size_t i = 0; i < AttackCount; i++) {
		RegisteredAttacks += RegisterAttack((Attack_t*)AttacksToRegister[i]);
	}

	LoadLuaAttacks(Lua, AttackRegistrar);

	BuildAttackList();

	return RegisteredAttacks;
}
