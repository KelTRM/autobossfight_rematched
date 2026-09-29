#include"../../debug/debug.h"
#include<stdlib.h>
#include<string.h>
#include<lua.h>

// returns true if found. false otherwise
// pushes the requested table from the registry onto the stack
int GetRegistryTable(lua_State *L, const char *Location) {
	int top = lua_gettop(L);

	size_t Length = strlen(Location);
	char *Text = malloc(Length+1);
	memcpy(Text, Location, Length+1);

	lua_pushvalue(L, LUA_REGISTRYINDEX);

	char *Start = Text;
	for (size_t i = 0; i <= Length; i++) {
//		write_debug(Debug, "@ char '%c' (%d)", Text[i], Text[i]);
		if (Text[i] == '.' || Text[i] == '\0') {
			Text[i] = '\0';

			int type = lua_getfield(L, -1, Start);
			write_debug(Debug, "Reading field %s", Start);
			if (type != LUA_TTABLE)
				goto error;

			lua_remove(L, -2);

			Start = Text+i+1;
			
		}
	}

	free(Text);
	return 1;

error:
	write_debug(Error, "Failed to read from table @ registry location %s. Failed to find table in field %s", Location, Start);
	free(Text);

	lua_settop(L, top);
	return 0;
}
