#include<string.h>
#include"../../entity.h"

extern Entity_t *Entities;
extern size_t EntityCount;

Entity_t *GetEntityFromName(const char *Name) {
	Entity_t *Entity = NULL;
	for (size_t i = 0; i < EntityCount; i++) {
		int r = strcmp(Entities[i].Name, Name);
		if (r != 0) continue;

		Entity = Entities + i;
		break;
	}

	return Entity;
}

