#include"attacks/attacks.h"
#include"attack_manager.h"
//#include"attacks/lua_attack_manager.h"
#include<registration_allocator.h>
#include<lua_load.h>
#include<registration.h>
#include<stddef.h>
#include<assert.h>
#include"debug/debug.h"
#include "utils/offset.h"

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
	RegistrationMgr_t Manager = OpenPluginAllocator(0);

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
	BlockID_t BlockCount = AllocatePlugin(&Manager, MaxID, &Plugin);
	
	assert(Plugin != INVALID_PLUGIN_ID);
	assert(BlockCount != 0);

	for (size_t i = 0; i < AttackCount; i++) {
		Attack_t *Attack = (Attack_t*)AttacksToRegister[i];
		write_debug(Info, "Registering attack %s @ ID %d", Attack->Identifier, Attack->ID);
		RegisteredAttacks += AddRegistrationToPlugin(
			&Manager,
			Plugin,
			Attack,
			Offset(Attack_t, ID)
		);
	}

	write_debug(InitAttacks, "registered %zu builtins", RegisteredAttacks);

	LoadLuaAttacks(Lua, &Manager);

//	size_t RegistrationCount = RegisterAttackPlugins(Manager, Registrar);
	RegisterPlugins(&Manager, &AttackRegistrar, 256);
	
	BuildAttackList();

	return RegisteredAttacks;
}
