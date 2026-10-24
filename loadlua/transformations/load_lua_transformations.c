#define USE_STD_STRLEN

#include<lua.h>
#include<stdlib.h>
#include<stddef.h>
#include<string.h>
#include"../../transformations/transformation_manager.h"
#include"../../transformations/default_transformation.h"
#include"../../registration/registration_allocator.h"
#include"../lua_bossfight.h"
#include"strings.h"
#include"types/plugin/lua_plugin.h"
#include"types/lua_types.h"
#include"../../utils/offset.h"

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

size_t CopyTransformation(Transformation_t *From, Transformation_t *To);

size_t RegisterLuaTransformations(void *LuaState, RegistrationMgr_t *Manager) {
	struct BossfightLuaState *Lua = LuaState;
	lua_State *L = Lua->L;

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
	
		type = lua_type(L, -2);
		write_verbose(Debug, "type(-2)=%s", lua_typename(L, type));

		while (lua_next(L, -2) != 0) {
			type = lua_type(L, -1);
			EntryCount++;

			type = lua_getfield(L, -1, "id");
			if (type == LUA_TNUMBER) {
				lua_Integer id = lua_tointeger(L, -1);
				if (MaxID < id) MaxID = id;
			}

			lua_pop(L, 2);
		}

		MaxID = max(EntryCount, MaxID);

		PluginID_t Plugin;
		size_t Blocks = AllocatePlugin(Manager, MaxID, &Plugin);
		if (Blocks == 0) {
			// well fuck
			lua_pop(L, 1);
			break;
		}

		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			Transformation_t tbl = ConvertTableToTransformation(
				L, i,
				lua_tostring(L, -2)
			);

			Transformation_t *t = malloc(sizeof(Transformation_t));
//			memcpy(t, &tbl, sizeof(tbl));
			CopyTransformation(&tbl, t);

			write_verbose(Debug, "ptr=%p, got copy of transformation \"%s\" (%s)",
					t, t->TransformationName, t->Identifier);

//			t->FormDescription = "lua";

			size_t Count = AddRegistrationToPlugin(
				Manager,
				Plugin, t,
				Offset(Transformation_t, ID),
				sizeof(tbl)
			);

			write_verbose(Debug, "Registered %d forms from Transformation %s",
					Count, tbl.Identifier);

			free(tbl.LuaData);
			if (Count == 0) {
				free(t->LuaData);
				free(t->TransformationName);
				free(t->Identifier);
			}

			Registrations += Count;
			lua_pop(L, 1);
		}
	}

	return Registrations;
}

int GetPluginEntry(const char *RegistrarName, const struct LuaPluginLocation *Location);

//typedef Health_t (*GetTransformedAttack_t)(Transformation_t *Self, Entity_t *From, Health_t BaseDamage);
//typedef Energy_t (*Maintain_t)(Transformation_t *Self, Entity_t *Possessor);

Health_t LuaGetDamage(Transformation_t *Self, Entity_t *From, Health_t BaseDamage);
Energy_t LuaMaintain(Transformation_t *Self, Entity_t *Possessor);

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
	
	GetFieldWithType(L, -1, "description", LUA_TSTRING);
	Form.FormDescription = lua_tostring(L, -1);
	lua_pop(L, 1);
	
	GetFieldWithType(L, -1, "int_name", LUA_TSTRING);
	Form.Identifier = (char*)lua_tostring(L, -1);
	lua_pop(L, 1);

	GetFieldWithType(L, -1, "cost", LUA_TNUMBER);
	Form.Cost = lua_tonumber(L, -1);
	lua_pop(L, 1);

	GetFieldWithType(L, -1, "upkeep", LUA_TNUMBER);
	Form.Upkeep = lua_tonumber(L, -1);
	lua_pop(L, 1);

	GetFieldWithType(L, -1, "multiplier", LUA_TNUMBER);
	Form.TransformationMultiplier = (double)(lua_tonumber(L, -1) * ATTACK_MULTIPLIER_BASE);
	lua_pop(L, 1);

	int type = lua_getfield(L, -1, "on_damage");

	Form.GetDamage = DefaultGetDamage;
	if (type == LUA_TFUNCTION)
		Form.GetDamage = LuaGetDamage;
	lua_pop(L, 1);

	type = lua_getfield(L, -1, "on_upkeep");

	Form.MaintainTransformation = DefaultMaintainTransformation;
	if (type == LUA_TFUNCTION)
		Form.MaintainTransformation = LuaMaintain;
	lua_pop(L, 2);

	write_verbose(Debug, "got entry of { %s,%s,%d,%d,%d }",
		Form.Identifier, Form.TransformationName,
		Form.Cost, Form.Upkeep, Form.TransformationMultiplier);

	Form.LuaData = malloc(sizeof(struct LuaPluginLocation));
	memcpy(Form.LuaData, &Location, sizeof(Location));


//	Form.LuaData = &Location;

	return Form;
}

// Creates a hard copy of a transformation
size_t CopyTransformation(Transformation_t *From, Transformation_t *To) {
	if (To == NULL || From == NULL)
		return 0;
	memcpy(To, From, sizeof(Transformation_t));
	
	To->TransformationName = malloc(strlen(From->TransformationName)+1);
	strcpy(To->TransformationName, From->TransformationName);
	
	To->FormDescription = malloc(strlen(From->FormDescription)+1);
	strcpy((char*)To->FormDescription, From->FormDescription);

	To->Identifier = malloc(strlen(From->Identifier)+1);
	strcpy(To->Identifier, From->Identifier);

	To->LuaData = malloc(sizeof(struct LuaPluginLocation));
	memcpy(To->LuaData, From->LuaData, sizeof(struct LuaPluginLocation));


	return sizeof(Transformation_t);
}

//typedef Health_t (*GetTransformedAttack_t)(Transformation_t *Self, Entity_t *From, Health_t BaseDamage);
Health_t LuaGetDamage(Transformation_t *Self, Entity_t *From, Health_t BaseDamage) {
	if (Self->LuaData == NULL) return BaseDamage;

	struct LuaPluginLocation *Form = Self->LuaData;
	lua_State *L = Form->L;

	GetPluginEntry("transformation", Form);
	lua_getfield(L, -1, "on_damage");

	lua_pushvalue(L, -2);		// self
	CreateEntityTable(L, From);	// from
	lua_pushinteger(L, BaseDamage);	// base
	
	lua_call(L, 3, 1);

	int type = lua_type(L, -1);
	TypeAssert("function on_damage()", LUA_TNUMBER, type);

	Health_t Result = lua_tointeger(L, -1);
	lua_pop(L, 2);

	return Result;
}

//typedef Energy_t (*Maintain_t)(Transformation_t *Self, Entity_t *Possessor);
Energy_t LuaMaintain(Transformation_t *Self, Entity_t *Possessor) {
	if (Self->LuaData == NULL) return Self->Upkeep;

	struct LuaPluginLocation *Form = Self->LuaData;
	lua_State *L = Form->L;

	GetPluginEntry("transformation", Form);
	lua_getfield(L, -1, "on_upkeep");

	lua_pushvalue(L, -2);			// self
	CreateEntityTable(L, Possessor);	// user
	lua_call(L, 2, 1);	// on_upkeep(self, user)
	
	TypeAssert("function on_upkeep()",
		LUA_TNUMBER, lua_type(L, -1));

	Energy_t Result = lua_tointeger(L, -1);
	lua_pop(L, 2);

	return Result;
}
