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
size_t RegisterPlugin(AttackMgr_t *Manager, lua_State *L);

size_t LoadLuaAttacks(void *LuaState, Registrar_t *Registrar) {
	struct BossfightLuaState *State = LuaState;

	size_t AttackCount = RegisterLuaPlugins(&State->Attacks.mgr, State->L);
	RegisterAttackPlugins(&State->Attacks.mgr, Registrar);

	return AttackCount;
}

size_t RegisterLuaPlugins(AttackMgr_t *Manager, lua_State *L) {
	int type = lua_getglobal(L, PluginRegistrationsName);
	if (type != LUA_TTABLE) return 0;

	size_t PluginCount = lua_rawlen(L, -1);
	for (size_t i = 0; i < PluginCount; i++) {
		lua_rawgeti(L, -1, i+1);
		RegisterPlugin(Manager, L);
		lua_pop(L, 1);
	}
}

Attack_t ConvertTableToAttack(lua_State *L, int idx);

// Plugin array @ top of stack
size_t RegisterPlugin(AttackMgr_t *Manager, lua_State *L) {
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
		Attack_t *LuaAttack = (Attack_t*)malloc(sizeof(Attack_t));
		*LuaAttack = ConvertTableToAttack(L, -1);
		
		lua_pop(L, 1);

		Attack_t **Attack = IndexPluginSpace(Manager, ID, LuaAttack->ID);
		if (Attack == NULL) continue;

		*Attack = LuaAttack;
		RegisteredAttacks++;
	}

	return RegisteredAttacks;
}

size_t RegisterAttackPlugins(AttackMgr_t *mgr, Registrar_t *Registrar) {

}
