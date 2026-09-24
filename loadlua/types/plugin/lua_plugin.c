#include<lua.h>

// takes in upvalues of 2 tables
//
// the table is full of default values for plugins.
// Anything that's nil will take the default value.
// Anything that shares a type with the default will override, and anything else will create an error.
int CreateLuaPlugin(lua_State *L) {
}
