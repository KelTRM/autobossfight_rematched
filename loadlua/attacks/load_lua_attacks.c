#include <assert.h>
#include<lua.h>
#include"../../registration/registration.h"
#include"../../debug/debug.h"
#include "attacks/lua_attack_manager.h"
#include<lua_bossfight.h>
#include<lua_load.h>
#include <stdlib.h>

extern const char *PluginRegistrationsName;

size_t RegisterAttackPlugins(AttackMgr_t *mgr, Registrar_t *Registrar);
size_t RegisterLuaPlugins(AttackMgr_t *Manager, lua_State *L);
size_t RegisterPlugin(AttackMgr_t *Manager, lua_State *L, PluginID_t PluginIndex);

size_t LoadLuaAttacks(void *LuaState, Registrar_t *Registrar) {
	struct BossfightLuaState *State = LuaState;

	size_t AttackCount = RegisterLuaPlugins(&State->Attacks.mgr, State->L);
	size_t RegistrationCount = RegisterAttackPlugins(&State->Attacks.mgr, Registrar);

	write_debug(LoadLuaAttacks, "got %lu attacks, registered %lu attacks.",
			AttackCount, RegistrationCount);

	return RegistrationCount;
}

size_t RegisterLuaPlugins(AttackMgr_t *Manager, lua_State *L) {
	int type = lua_getglobal(L, PluginRegistrationsName);
	if (type != LUA_TTABLE) return 0;

	size_t PluginCount = lua_rawlen(L, -1);
	for (size_t i = 0; i < PluginCount; i++) {
		lua_rawgeti(L, -1, i+1);
		RegisterPlugin(Manager, L, i+1);
		lua_pop(L, 1);
	}
}

Attack_t ConvertTableToAttack(lua_State *L, int idx, const char *Key, size_t PluginIdx);

// Plugin array @ top of stack
size_t RegisterPlugin(AttackMgr_t *Manager, lua_State *L, PluginID_t Index) {
	size_t RequiredAttacks=0;

	// iterate over plugin to get attack count
	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		++RequiredAttacks;
		lua_pop(L, 1);
	}

	PluginID_t ID;
	size_t MaxAttacks = AllocateAttackPlugin(Manager, RequiredAttacks, &ID);

	write_debug(RegisterPlugin, "Recieved plugin with %lu attacks. Got %lu attacks back",
			RequiredAttacks, MaxAttacks * ATTACK_BLOCK_SIZE);

	if (MaxAttacks * ATTACK_BLOCK_SIZE < RequiredAttacks) {
		// just stick to allocated what can be
		RequiredAttacks = MaxAttacks * ATTACK_BLOCK_SIZE;
	}

	size_t RegisteredAttacks = 0;

	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		if (RegisteredAttacks >= RequiredAttacks) break;
	
		const char *Identifier = NULL;

		// get identifier
		int IdentifierType = lua_type(L, -2);
		if (IdentifierType == LUA_TSTRING) {
			Identifier = lua_tolstring(L, -2, NULL);
		}

		Attack_t *LuaAttack = (Attack_t*)malloc(sizeof(Attack_t));
		*LuaAttack = ConvertTableToAttack(L, -1, Identifier, Index);
		
		lua_pop(L, 1);
//		write_debug(RegisterPlugin, "got attack identifier = %s", LuaAttack->Identifier);
//		write_debug(RegisterPlugin, "got attack display name = %s", LuaAttack->AttackName);
//		Attack_t **Attack = IndexPluginSpace(Manager, ID, LuaAttack->ID);
//		if (Attack == NULL) continue;

//		write_debug(RegisterPlugin, "adding attack from plugin %d @ id=%d", ID, LuaAttack->ID);

		RegisteredAttacks += AddAttackToPlugin(Manager, ID, LuaAttack);
//		*Attack = LuaAttack;
//		RegisteredAttacks++;
	}

	return RegisteredAttacks;
}

size_t RegisterAttackPlugins(AttackMgr_t *mgr, Registrar_t *Registrar) {
	return RegisterPluginAttacks(mgr, Registrar, 255);
}
