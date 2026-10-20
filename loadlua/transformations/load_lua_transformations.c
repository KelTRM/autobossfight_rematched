#include<lua.h>
#include<stdlib.h>
#include<stddef.h>
#include<string.h>
#include"../../transformations/transformation_manager.h"
#include"../../registration/registration_allocator.h"
#include"../lua_bossfight.h"
#include "strings.h"
#include "types/plugin/lua_plugin.h"

//	PluginField(L, "name",		LUA_TSTRING, lua_pushnil(L), 0, "name");
//	PluginField(L, "cost",		LUA_TNUMBER, lua_pushnil(L), 0, "cost");
//	PluginField(L, "upkeep",	LUA_TNUMBER, lua_pushnil(L), 0, "upkeep");
//	PluginField(L, "multiplier",	LUA_TNUMBER, lua_pushnil(L), 0, "multiplier");
//	PluginField(L, "on_damage",	LUA_TFUNCTION, lua_pushnil(L), 1, "on_damage");
//	PluginField(L, "on_upkeep",	LUA_TFUNCTION, lua_pushnil(L), 1, "on_upkeep");

#define GetFieldWithType(L, idx, name, type)	do {	\
	int tp = lua_getfield((L), (idx), (name));	\
	if ((type) != tp) {				\
		TypeError((L), (type), tp);		\
	}						\
} while (0);

int GetRegistryTable(lua_State *L, const char *Location);
Transformation_t ConvertTableToTransformation(lua_State *L, PluginID_t ID, const char *Name);

size_t RegisterLuaTransformations(void *LuaState, RegistrationMgr_t *Manager) {
	struct LuaPluginLocation *State = LuaState;
	lua_State *L = State->L;

	GetRegistryTable(L, "bossfight.plugins.transformation");
	size_t PluginCount = lua_rawlen(L, -1);

	size_t Registrations = 0;

	for (size_t i = 1; i <= PluginCount; i++) {
		int type = lua_rawgeti(L, -1, i);
		if (type != LUA_TTABLE) {
			continue;
		}

		int EntryCount = 0;
		int MaxID = 0;

		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			EntryCount++;

			type = lua_getfield(L, -1, "id");
			if (type != LUA_TNUMBER) {
				lua_pop(L, 1);
				continue;
			};

			lua_Integer id = lua_tointeger(L, -1);
			if (MaxID < id) MaxID = id;
			lua_pop(L, 2);
		}

		MaxID = max(EntryCount, MaxID);

		PluginID_t Plugin;
		size_t Blocks = AllocatePlugin(Manager, MaxID, &Plugin);
		write_verbose(Debug, "Got plugin @ idx=%zu with %zu blocks & PluginID=%ld.",
				i, Blocks, Plugin);

		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			Transformation_t tbl = ConvertTableToTransformation(
				L, i,
				lua_tostring(L, -2)
			);
			Transformation_t *t = malloc(sizeof(Transformation_t));
			memcpy(t, &tbl, sizeof(tbl));

			size_t Count = AddRegistrationToPlugin(
				Manager,
				Plugin, &t,
				offsetof(Transformation_t, ID),
				sizeof(tbl)
			);
		}
	}

}

int GetPluginEntry(const char *RegistrarName, const struct LuaPluginLocation *Location);

//typedef Health_t (*GetTransformedAttack_t)(Transformation_t *Self, Entity_t *From, Health_t BaseDamage);
//typedef Energy_t (*Maintain_t)(Transformation_t *Self, Entity_t *Possessor);

Transformation_t ConvertTableToTransformation(lua_State *L, PluginID_t ID, const char *Name) {
	struct LuaPluginLocation Location = {
		.L=L,
		.PluginID=ID,
		.PluginKey=Name
	};

	Transformation_t Form = { 0 };

	int Result = GetPluginEntry("transformation", &Location);
	if (Result == 0)
		return Form;

	GetFieldWithType(L, -1, "name", LUA_TSTRING);
	Form.TransformationName = (char*)lua_tostring(L, -1);
	lua_pop(L, 1);

	GetFieldWithType(L, -1, "cost", LUA_TNUMBER);
	Form.Cost = lua_tonumber(L, -1);
	lua_pop(L, 1);

	GetFieldWithType(L, -1, "upkeep", LUA_TNUMBER);
	Form.Upkeep = lua_tonumber(L, -1);
	lua_pop(L, -1);

	GetFieldWithType(L, -1, "multiplier", LUA_TNUMBER);
	Form.TransformationMultiplier = lua_tonumber(L, -1);
	lua_pop(L, 1);

	Form.LuaData = malloc(sizeof(struct LuaPluginLocation));
	memcpy(Form.LuaData, &Location, sizeof(Location));

	return Form;
}
