#include<math.h>
#include<lua.h>
#include"../../rng.h"

void AssertParameters(lua_State *L, const char *Prototype, int ParamCount, ...);

int LuaRandom(lua_State *L) {
	AssertParameters(L, "bossfight:rand(min,max)", 3,
			LUA_TTABLE, LUA_TNUMBER, LUA_TNUMBER);

	lua_Number Lower = lua_tonumber(L, 2);
	lua_Number Upper = lua_tonumber(L, 3);

	uint64_t Random = GetRandomInt64();
	double RandomDouble = *(double*)&Random;

//	RandomDouble -= Lower;
//	RandomDouble /= floor(RandomDouble / (Upper-Lower));

	lua_pushnumber(L, RandomDouble + Lower);
	return 1;
}
