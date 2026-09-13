#include<stdlib.h>
#include<assert.h>
#include<lua.h>
#include"../../registration/registration_allocator.h"
#include"../../debug/debug.h"
#include"../../attacks/attack.h"
#include<lua_bossfight.h>
#include<lua_load.h>

#define max(a,b)	((a)>(b)?(a):(b))

// extern const char *PluginRegistrationsName;

size_t RegisterAttackPlugins(RegistrationMgr_t *mgr, Registrar_t *Registrar);
size_t RegisterLuaPlugins(RegistrationMgr_t *Manager, lua_State *L);
size_t RegisterPlugin(RegistrationMgr_t *Manager, lua_State *L, PluginID_t PluginIndex);

size_t LoadLuaAttacks(void *LuaState, RegistrationMgr_t *Manager) {
	struct BossfightLuaState *State = LuaState;

	size_t AttackCount = RegisterLuaPlugins(Manager, State->L);
//	size_t RegistrationCount = RegisterAttackPlugins(Manager, Registrar);
	write_debug(LoadLuaAttacks, "got %lu attacks.",
			AttackCount);

	return AttackCount;
}

int GetAttackPluginsTable(lua_State *L);

size_t RegisterLuaPlugins(RegistrationMgr_t *Manager, lua_State *L) {
	int r = GetAttackPluginsTable(L);
	if (r == 0) {
		lua_pop(L, 1);
		return 0;
	}

	size_t PluginCount = lua_rawlen(L, -1);
	for (size_t i = 0; i < PluginCount; i++) {
		lua_rawgeti(L, -1, i+1);
		RegisterPlugin(Manager, L, i+1);
		lua_pop(L, 1);
	}

//	lua_pop(L, 1);
	return PluginCount;
}

Attack_t ConvertTableToAttack(lua_State *L, int idx, const char *Key, size_t PluginIdx);

// Plugin array @ top of stack
size_t RegisterPlugin(RegistrationMgr_t *Manager, lua_State *L, PluginID_t Index) {
	size_t RequiredAttacks=0;
	AttackID_t MaxID = 0;

	// iterate over plugin to get attack count
	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		int type = lua_getfield(L, -1, "id");
		if (type == LUA_TNUMBER)
			MaxID = max(MaxID, lua_tonumber(L, -1));

		++RequiredAttacks;
		lua_pop(L, 2);
	}

	RequiredAttacks = max(MaxID, RequiredAttacks);
	write_debug(Info, "RequiredAttacks=%zu", RequiredAttacks);

	PluginID_t ID;
	size_t MaxAttacks = AllocatePlugin(Manager, RequiredAttacks, &ID);

	write_debug(RegisterPlugin, "Recieved plugin with %lu attacks. Got %lu attacks back",
			RequiredAttacks, MaxAttacks * BLOCK_SIZE);

	if (MaxAttacks * BLOCK_SIZE < RequiredAttacks) {
		// just stick to allocated what can be
		RequiredAttacks = MaxAttacks * BLOCK_SIZE;
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

		write_debug(RegisterPlugin, "adding attack from plugin %d @ id=%d", ID, LuaAttack->ID);
		RegisteredAttacks += AddRegistrationToPlugin(
			Manager,
			ID,
			LuaAttack->ID,
			LuaAttack
		);
	}

	return RegisteredAttacks;
}

size_t RegisterAttackPlugins(RegistrationMgr_t *mgr, Registrar_t *Registrar) {
	return RegisterPlugins(mgr, Registrar, 255);
}
