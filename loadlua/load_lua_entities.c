#include"../entity.h"
#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
// lua shit
#include<lua.h>
#include<lauxlib.h>
#include<lualib.h>

int CheckLuaEntities(void) {
	// check for lua/entities.lua file. This file is required for the game to function.
	FILE *tmp = fopen("lua/entities.lua", "r");
	if (tmp == NULL)
		return -1;

	fclose(tmp);

	return 0;
}

// i have no idea what i'm doing, so it's blank for now
size_t LoadLuaEntities(Entity_t **Entities) {
	// read contents of entities lua file
	FILE *f = fopen("lua/entities.lua", "r");

	// no entities if entities.lua isn't
	if (f == NULL)
		return 0;

	fseek(f, 0, SEEK_END);
	size_t FileSize = ftell(f);

	// buffer contains file contents of entities.lua
	void *Buffer = malloc(FileSize);
	fread(Buffer, FileSize, 1, f);

	free(Buffer);
	fclose(f);

	(void)Entities;
	return 0;
}
