#include<lua.h>
#include"../../registration/registration.h"
#include<lua_bossfight.h>
#include<lua_load.h>



size_t LoadLuaAttacks(void *LuaState, Registrar_t Registrar) {
	struct BossfightLuaState *State = LuaState;

	size_t AttackCount = RegisterLuaAttacks(State->Attacks.mgr, State->L);
	RegisterAttackPlugins();
}
