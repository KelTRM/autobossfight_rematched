#include<stdio.h>
#include"entity.h"
#include"rng.h"
//#include"ui/console_manager/buffer.h"
#include<ui.h>
#include"utils/sleep.h"
#include"debug/debug.h"

//#ifdef __linux__
//#include<unistd.h>
//#endif
// char *CleanseFilePath(const char *Filename);
int Init(void);

int IsGameOngoing(void);

extern const int PlayerCount;
extern const int BossCount;

// const int ParticipantCount = PlayerCount + BotCount;

uint64_t Round = 0;

//void InitPlayers(void);
int InitAttacks(void *Lua);
size_t InitTransformations(void);

void *InitLua(void);
void InitEntities(void *Lua);

extern Entity_t *Entities;
extern size_t EntityCount;

int main(void) {
	BUFHANDLE b = CreateBuffer();
	SwitchBuffer(b);

	init_debug();
	define_debug_flush_location(fopen("DEBUG.txt", "w"));

	// should initialize the lua state
	void *Lua = InitLua();

	Init();
	InitEntities(Lua);

	int Attacks = InitAttacks(Lua);
	printf("Initialized %d attacks.\n", Attacks);

	int Transformations = InitTransformations();
	printf("Initialized %d transformations.\n", Transformations);

	int Turn = 0;

	while (IsGameOngoing()) {
		uint64_t PriorRound = Round;
		Round = (Turn / EntityCount) + 1;

		if (Round != PriorRound) {
			EnergizeEntities();
		}

		int CurrentPlayerID = Turn % EntityCount;

		Entity_t *CurrentPlayer = &Entities[CurrentPlayerID];

		CurrentPlayer->EntityTransformation->MaintainTransformation(
			CurrentPlayer->EntityTransformation,
			CurrentPlayer
		);

		if (IsEntityAlive(CurrentPlayer)) {
			CurrentPlayer->TakeTurn(CurrentPlayer, Round);
			sleep(1500);
		}

		Turn++;
	}

	printf("Game over.\n");
}

// temporary implementation
int IsGameOngoing(void) {
	for (size_t i = 0; i < EntityCount; i++) {
		Entity_t *Entity = &Entities[i];

		if (!IsEntityAlive(Entity))
			continue;

		if (HasLivingEnemies(Entity))
			return 1;
	}

	return 0;
}
