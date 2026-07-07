#include<lua.h>
#include<stdio.h>
#include<stddef.h>
#include"../entity.h"

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
	(void)Entities;
	return 0;
}
