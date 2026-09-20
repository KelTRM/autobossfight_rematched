#include<stdarg.h>
#include<assert.h>
#include<lua.h>
#include<lauxlib.h>
#include"../../entity.h"
#include"../../debug/debug.h"

#define PROTO_GET_HEALTH	"entity:GetHealth()"
#define PROTO_GET_ENERGY	"entity:GetEnergy()"
#define PROTO_DRAIN_ENERGY	"entity:DrainEnergy(energy)"
#define PROTO_HEAL		"entity:Heal(target, health)"
#define PROTO_ATTACK		"entity:Attack(target, health)"
#define PROTO_LIVING		"entity:Living()"

#define SetNumberField(name, value, idx)	lua_pushnumber(L, value);	\
						lua_setfield(L, idx, name)

int GetTempEntity(lua_State *L, int idx, Entity_t *Entity);

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
int Entity_DrainEnergy(lua_State *L);
int Entity_Heal(lua_State *L);
int Entity_Attack(lua_State *L);
int Entity_Living(lua_State *L);
int Entity_GetEnemies(lua_State *L);

void CreateEntityTable(lua_State *L, Entity_t *Entity) {
	lua_newtable(L);

	luaL_Reg fns[] = {
		{ "GetHealth", Entity_GetHealth },
		{ "GetEnergy", Entity_GetEnergy },
		{ "DrainEnergy", Entity_DrainEnergy },
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

	SetNumberField("r", Entity->EntityColor.r, -2);
	SetNumberField("g", Entity->EntityColor.g, -2);
	SetNumberField("b", Entity->EntityColor.b, -2);

	lua_setfield(L, -2, "color");

	SetNumberField("attack", Entity->Attack, -2);
	SetNumberField("energy", Entity->Energy, -2);
	SetNumberField("healing_minimum", Entity->HealingMinimum, -2);
	SetNumberField("healing_maximum", Entity->HealingMaximum, -2);
	SetNumberField("hp", Entity->HealthPoints, -2);
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

	int type = lua_getfield(L, 1, "energy");

	if (type != LUA_TNUMBER) {
		lua_pop(L, 1);
		lua_pushnil(L);
	}

	return 1;
}

int Entity_DrainEnergy(lua_State *L) {
	AssertParameters(L, PROTO_DRAIN_ENERGY, 2,
			LUA_TTABLE, LUA_TNUMBER);

	int type = lua_getfield(L, 1, "energy");
	if (type != LUA_TNUMBER) {
		lua_pushliteral(L, "expected entity.energy of type number");
	}

	lua_Number Energy = lua_tonumber(L, 2);
	lua_Number DrainAmount = lua_tonumber(L, -1);
	lua_pop(L, 1);

	Energy -= DrainAmount;
	
	if (Energy < 0) Energy = 0;
	lua_pushnumber(L, Energy);

	lua_setfield(L, 1, "energy");

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

int Entity_ToString(lua_State *L) {
	AssertParameters(L, "tostring(v)", 1, LUA_TTABLE);

	Entity_t Entity;
	GetTempEntity(L, 1, &Entity);


}

