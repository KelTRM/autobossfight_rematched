#include<lua.h>
#include"../entity.h"
#include<assert.h>

//```lua
//entity:GetHealth()          -- returns number of health points
//entity:GetEnergy()          -- returns energy as a percentage
//entity:Heal(hp)             -- heals entity by hp, returns amount healed
//entity:Attack(target,hp)    -- removes hp from target's health
//entity:Living()             -- returns whether the entity is alive
//entity:GetEnemies()         -- gets the enemies of the entity
//```

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




