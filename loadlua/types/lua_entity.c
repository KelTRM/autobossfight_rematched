#include<stdarg.h>
#include<assert.h>
#include<lua.h>
#include<lauxlib.h>
#include"../entity.h"
#include"../../debug/debug.h"

#define PROTO_GET_HEALTH	"entity:GetHealth()"
#define PROTO_GET_ENERGY	"entity:GetEnergy()"
#define PROTO_HEAL		"entity:Heal(target, health)"
#define PROTO_ATTACK		"entity:Attack(target, health)"
#define PROTO_LIVING		"entity:Living()"

//```lua
//entity:GetHealth()          -- returns number of health points
//entity:GetEnergy()          -- returns energy as a percentage
//entity:Heal(hp)             -- heals entity by hp, returns amount healed
//entity:Attack(target,hp)    -- removes hp from target's health
//entity:Living()             -- returns whether the entity is alive
//entity:GetEnemies()         -- gets the enemies of the entity
//```
int Entity_GetHealth(lua_State *L);
int Entity_GetEnergy(lua_State *L);
int Entity_Heal(lua_State *L);
int Entity_Attack(lua_State *L);
int Entity_Living(lua_State *L);
int Entity_GetEnemies(lua_State *L);

void CreateEntityTable(lua_State *L, Entity_t *Entity) {
	lua_newtable(L);

/*	FIELDS TO ADD
 -----------------------------------
	Entity->Name;
	Entity->EntityColor;
	Entity->Attack;
	Entity->Energy;
	Entity->HealingMinimum;
	Entity->HealingMaximum;
	Entity->HealthPoints;

	entity:GetHealth()
	entity:GetEnergy()
	entity:Heal(hp)
	entity:Attack(target, hp)
	entity:Living()
	entity:GetEnemies()
 -----------------------------------
*/

	luaL_Reg fns[] = {
		{ "GetHealth", Entity_GetHealth },
		{ "GetEnergy", Entity_GetEnergy },
		{ "Heal", Entity_Heal },
		{ "Attack", Entity_Attack },
		{ "Living", Entity_Living },
		{ NULL, NULL }
	};
	luaL_setfuncs(L, fns, 0);

	// name
	lua_pushstring(L, Entity->Name);
	lua_setfield(L, -2, "name");

	// color
	lua_newtable(L);


	lua_pushnumber(L, Entity->EntityColor.r);
	lua_pushnumber(L, Entity->EntityColor.g);
	lua_pushnumber(L, Entity->EntityColor.b);

	lua_setfield(L, -4, "r");
	lua_setfield(L, -3, "g");
	lua_setfield(L, -2, "b");

	lua_setfield(L, -2, "color");

	// attack
	lua_pushnumber(L, Entity->Attack);
	lua_setfield(L, -2, "attack");

	// energy
	lua_pushnumber(L, Entity->Energy);
	lua_setfield(L, -2, "energy");

	// healing_minimum
	lua_pushnumber(L, Entity->HealingMinimum);
	lua_setfield(L, -2, "healing_minimum");

	// healing_maximum
	lua_pushnumber(L, Entity->HealingMaximum);
	lua_setfield(L, -2, "healing_maximum");
	
	// hp
	lua_pushnumber(L, Entity->HealthPoints);
	lua_setfield(L, -2, "hp");
}

// gets entity from top of table, converts it to C entity
Entity_t GetEntityFromTable(lua_State *L) {
	// this code is very DRY (do repeat yourself)
	Entity_t Entity = { 0 };

	int type = lua_type(L, -1);
	assert(type == LUA_TTABLE);

	// name
	lua_getfield(L, -1, "name");
	Entity.Name = lua_tostring(L, -1);
	lua_pop(L, 1);

	// color
	lua_getfield(L, -1, "color");

	lua_getfield(L, -1, "r");
	Entity.EntityColor.r = lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, -2, "g");
	Entity.EntityColor.g = lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, -3, "b");
	Entity.EntityColor.b = lua_tonumber(L, -1);
	lua_pop(L, 2);

	// attack
	lua_getfield(L, -1, "attack");
	Entity.Attack = lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, -1, "energy");
	Entity.Energy = lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, -1, "healing_minimum");
	Entity.HealingMinimum = lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, -1, "healing_maximum");
	Entity.HealingMaximum = lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, -1, "hp");
	Entity.HealthPoints = lua_tonumber(L, -1);
	lua_pop(L, 1);

	return Entity;
}

void AssertParameters(lua_State *L, const char *Prototype, int ParamCount, ...) {
	int top = lua_gettop(L);
	write_debug(AssertParameters, "top=%d; ParamCount=%d", top, ParamCount);
	if (top != ParamCount) {
		write_debug(AssertParameters, "top(%d)!=ParamCount(%d)", top, ParamCount);
		lua_pushfstring(L, "expected %s", Prototype);
		lua_error(L);
	}

	va_list args;
	va_start(args, ParamCount);

	for (int i = 1; i <= ParamCount; i++) {
		int ParamType = va_arg(args, int);
		int TrueParam = lua_type(L, i);

		if (ParamType != TrueParam) {
			lua_pushfstring(L, "expected type %s. got %s instead",
				Prototype,
				lua_typename(L, ParamType), lua_typename(L, TrueParam
			));
			lua_error(L);
		}
	}

	va_end(args);
}

int Entity_GetHealth(lua_State *L) {
	AssertParameters(L, PROTO_GET_HEALTH, 1,
			LUA_TTABLE);

	int type = lua_getfield(L, 1, "hp");
	if (type != LUA_TNUMBER) {
		lua_pop(L, 1);
		lua_pushnil(L);
	}

	return 1;
//	int top = lua_gettop(L);
//	if (top != 1) {
//		lua_pushliteral(L, "expected entity:GetHealth()");
//		lua_error(L);
//	}

//	if (lua_type(L, 1) != LUA_TTABLE) {
//		lua_pushliteral(L, "expected entity:GetHealth()");
//		lua_error(L);
//	}
}

int Entity_GetEnergy(lua_State *L) {
	AssertParameters(L, PROTO_GET_ENERGY, 1,
			LUA_TTABLE);
	int top = lua_gettop(L);
	if (top != 1) {
		lua_pushliteral(L, "expected entity:GetEnergy()");
		lua_error(L);
	}
	return 0;
}

int Entity_Heal(lua_State *L) {
	AssertParameters(L, PROTO_HEAL, 3,
			LUA_TTABLE, LUA_TTABLE, LUA_TNUMBER);
	return 0;
}

int Entity_Attack(lua_State *L) {
	AssertParameters(L, PROTO_ATTACK, 3,
			LUA_TTABLE, LUA_TTABLE, LUA_TNUMBER);

	int type;
	type = lua_getfield(L, 2, "hp");
	if (type != LUA_TNUMBER) {
		lua_pushliteral(L, "expected entity.hp of type number");
		lua_error(L);
	}

	lua_Number hp = lua_tonumber(L, -1);
	lua_pop(L, 1);
	
//	type = lua_getfield(L, 1, "attack");
//	if (type != LUA_TNUMBER) {
//		lua_pushliteral(L, "expected entity.attack of type number");
//		lua_error(L);
//	}

	lua_Number damage = lua_tonumber(L, 3);

	if (damage < 0) damage = 0;

	Health_t HP = hp - damage;
	if (HP > hp) HP = 0;	// underflow protection

	lua_pushnumber(L, HP);
	lua_setfield(L, 2, "hp");

	lua_pushnumber(L, hp - HP);
	return 1;
}

int Entity_Living(lua_State *L) {
	AssertParameters(L, PROTO_LIVING, 1, LUA_TTABLE);

	Entity_GetHealth(L);

	if (lua_type(L, -1) == LUA_TNIL) {
		lua_pop(L, 1);
		lua_pushboolean(L, 0);
	}

	lua_Number hp = lua_tonumber(L, -1);
	if (hp > 0) hp = 1;
	else hp = 0;

	lua_pushboolean(L, hp);
	return 1;
}

//int Entity_GetEnemies(lua_State *L) {
//	
//	return 0;
//}

