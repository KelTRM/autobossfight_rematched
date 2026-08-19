#include<lua.h>
#include"../../registration/registration.h"
#include<lua_bossfight.h>
#include<lua_load.h>

size_t RegisterAttackPlugins(AttackMgr_t *mgr, Registrar_t *Registrar);
size_t RegisterLuaPlugins(AttackMgr_t *Manager, lua_State *L);

size_t LoadLuaAttacks(void *LuaState, Registrar_t *Registrar) {
	struct BossfightLuaState *State = LuaState;

	size_t AttackCount = RegisterLuaPlugins(&State->Attacks.mgr, State->L);
	RegisterAttackPlugins(&State->Attacks.mgr, Registrar);

	return AttackCount;
}

size_t RegisterLuaPlugins(AttackMgr_t *Manager, lua_State *L) {

}

size_t RegisterAttackPlugins(AttackMgr_t *mgr, Registrar_t *Registrar) {

}
