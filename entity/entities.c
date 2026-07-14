#include"entity_manager.h"
#include"../debug/debug.h"
#include<lua_load.h>
#include<stdlib.h>
#include<stddef.h>
#include<assert.h>

size_t PlayerCount = 0;
size_t BossCount = 0;

Entity_t *Entities;
size_t EntityCount;

#define ArrayLength(array)	(sizeof(array) / sizeof(*array))

struct PlayerDefinition PlayerDefinitions[] = {
	// players go here
	{ .Name="TailsKirbo",	.HP=10000,	.Heal_Min=2500,	.Heal_Max=7500,	.Color=0xFF6400 },
	{ .Name="KelTRM",		.HP=10000,	.Heal_Min=2500,	.Heal_Max=7500,	.Color=0xFF0000 },
	{ .Name="SonicKirbo",	.HP=10000,	.Heal_Min=2500,	.Heal_Max=7500,	.Color=0x00FFFF },
};

struct BossDefinition BossDefinitions[] = {
	// boss(es) go here
	{ .Name="Cornelius",	.HP=325000 },
//	{ .Name="Cornelius Jr.",	.HP=100000 }
};

void InitEntities(void) {
	DefMgr_t Players;
	DefMgr_t Bosses;

	size_t LoadedEntities = LoadLuaEntities(&Players, &Bosses);
	write_debug(InitEntities, "Loaded %zu entity definitions from lua.", LoadedEntities);

	if (LoadedEntities == 0) {
		write_debug(InitEntities, "No entities loaded. exiting...");
		exit(-1);
	}

//	Entity_t *EntitiesTmp;
//	size_t LoadedEntities = LoadLuaEntities(&EntitiesTmp);
//	assert(LoadedEntities == 0);

//	PlayerCount = ArrayLength(PlayerDefinitions);
//	BossCount = ArrayLength(BossDefinitions);

	PlayerCount = Players.DefinitionCount;
	BossCount = Bosses.DefinitionCount;

	write_debug(InitEntities, "read %zu players & %zu bosses", PlayerCount, BossCount);

	EntityCount = PlayerCount + BossCount;
	Entities = calloc(EntityCount, sizeof(Entity_t));

	for (size_t i = 0; i < PlayerCount; i++) {
//		struct PlayerDefinition *Player = &PlayerDefinitions[i];
		struct PlayerDefinition *Player = &Players.PlayerDefinition[i];//&PlayerDefinitions[i];
		Entities[i] = CreatePlayer(
				Player->Name,
				Player->HP,
				Player->Heal_Min,
				Player->Heal_Max,
				GetColorFromHex(Player->Color)
		);
	}

	for (size_t i = 0; i < BossCount; i++) {
		struct BossDefinition *Boss = &Bosses.BossDefinition[i];
		Entities[PlayerCount+i] = CreateBoss(Boss->Name, Boss->HP);

		for (size_t j = 0; j < PlayerCount; j++) {
			AddEntityEnemy(&Entities[j], &Entities[PlayerCount+i]);
			AddEntityEnemy(&Entities[PlayerCount+i], &Entities[j]);
		}
	}
}
