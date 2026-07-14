#include"../debug/debug.h"
#include<lua.h>

int InitializeEntityManagerTable(lua_State *L, int Index) {
	// get the absolute position of the table because i don't wanna have to
	// deal with the relative shit with unknown formats for Index or whatever the fuck
	Index = lua_absindex(L, Index);

	write_debug(EntityManager, "Writing players table...");

	lua_newtable(L); // Players table
	lua_setfield(L, Index, "Players");
	
	write_debug(EntityManager, "Writing bosses table...");

	lua_newtable(L); // Boss table
	lua_setfield(L, Index, "Bosses");

	return 0;
}
