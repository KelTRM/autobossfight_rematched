#include<stdlib.h>
#include<stddef.h>
#include"../entity.h"

Entity_t CreatePlayer(const char *Name, Health_t MaxHP, Health_t MinHeal, Health_t MaxHeal);

extern const int PlayerCount;
extern const int BotCount;

extern Entity_t *Entities;
extern size_t EntityCount;

void InitEntities() {
	EntityCount = PlayerCount + BotCount;
	Entities = calloc(EntityCount, sizeof(Entity_t));

	for (int i = 0; i < PlayerCount; i++) {
		Entities[0] = CreatePlayer("PlayerI", 30000, 1000, 1000);
	}
}
