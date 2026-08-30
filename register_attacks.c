#include"attacks/attacks.h"
#include"attack_manager.h"
#include"attacks/lua_attack_manager.h"
#include<lua_load.h>
#include<registration.h>
#include<stddef.h>
#include<assert.h>
#include"debug/debug.h"

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
	AttackMgr_t Manager = OpenAttackAllocator(0);

	// keep around for now
	InitAttackRegistrar();

	size_t AttackCount = sizeof(AttacksToRegister) / sizeof(*AttacksToRegister);
	size_t RegisteredAttacks = 0;

	AttackID_t MaxID = 0;

	for (size_t i = 0; i < AttackCount; i++) {
//		RegisteredAttacks += RegisterAttack((Attack_t*)AttacksToRegister[i]);
		if (AttacksToRegister[i]->ID > MaxID)
			MaxID = AttacksToRegister[i]->ID;
	}

	write_debug(InitAttacks, "MaxID = %lu", MaxID);

	PluginID_t Plugin;
	BlockID_t BlockCount = AllocateAttackPlugin(&Manager, MaxID, &Plugin);

	assert(BlockCount != 0);

	for (size_t i = 0; i < AttackCount; i++) {
		RegisteredAttacks += AddAttackToPlugin(&Manager, Plugin, AttacksToRegister[i]);

	}

	write_debug(InitAttacks, "registered %zu builtins", RegisteredAttacks);

	LoadLuaAttacks(Lua, &Manager);

//	size_t RegistrationCount = RegisterAttackPlugins(Manager, Registrar);
	RegisterPluginAttacks(&Manager, &AttackRegistrar, 256);
	
	BuildAttackList();

	return RegisteredAttacks;
}
