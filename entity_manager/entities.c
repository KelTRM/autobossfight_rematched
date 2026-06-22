#include<stdlib.h>
#include<stddef.h>
#include"../entity.h"

extern const int PlayerCount;
extern const int BotCount;

Entity_t *Entities;
size_t EntityCount;

#define ArrayLength(array)	(sizeof(array) / sizeof(*array))

struct PlayerDefinition {
	const char *Name;		// name of the player
	const Health_t HP;		// health of the player
	const Health_t Heal_Min;	// minimum amount the player may heal another by
	const Health_t Heal_Max;	// maximum amount the player may heal another by
	
	const uint32_t Color;
} PlayerDefinitions[] = {
	// players go here
	{ .Name="TailsKirbo",	.HP=10000,	.Heal_Min=2500,		.Heal_Max=7500,		.Color=0xFF6400 },
	{ .Name="KelTRM",		.HP=10000,	.Heal_Min=2500,		.Heal_Max=7500,		.Color=0xFF0000 },
	{ .Name="SonicKirbo",	.HP=10000,	.Heal_Min=2500,		.Heal_Max=7500,		.Color=0x00FFFF }
};

struct BossDefinition {
	const char *Name;
	const Health_t HP;
	// no healing because boss bad
} BossDefinitions[] = {
	// boss(es) go here
	{ .Name="Cornelius",	.HP=325000 },
	{ .Name="Cornelius Jr.",	.HP=100000 }
};

void InitEntities() {
	size_t PlayerCount = ArrayLength(PlayerDefinitions);
	size_t BossCount = ArrayLength(BossDefinitions);

	EntityCount = PlayerCount + BossCount;
	Entities = calloc(EntityCount, sizeof(Entity_t));

	for (int i = 0; i < PlayerCount; i++) {
		struct PlayerDefinition *Player = &PlayerDefinitions[i];
		Entities[i] = CreatePlayer(
				Player->Name,
				Player->HP,
				Player->Heal_Min,
				Player->Heal_Max,
				GetColorFromHex(Player->Color)
		);
	}

	for (int i = 0; i < BossCount; i++) {
		struct BossDefinition *Boss = &BossDefinitions[i];
		Entities[PlayerCount+i] = CreateBoss(Boss->Name, Boss->HP);

		for (int j = 0; j < PlayerCount; j++) {
			AddEntityEnemy(&Entities[j], &Entities[PlayerCount+i]);
			AddEntityEnemy(&Entities[PlayerCount+i], &Entities[j]);
		}
	}
}
