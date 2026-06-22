#include<stdio.h>
#include"entity.h"
#include"rng.h"
//#include"ui/console_manager/buffer.h"
#include"ui/ui.h"
#include"utils/sleep.h"

//#ifdef __linux__
//#include<unistd.h>
//#endif

int Init();

int IsGameOngoing();

const int PlayerCount = 3;
const int BotCount = 1;

const int ParticipantCount = PlayerCount + BotCount;

int Round = 0;

void InitPlayers();
int InitAttacks();

void InitEntities();

extern Entity_t *Entities;
extern size_t EntityCount;

int main() {
	Init();
	InitEntities();

	int Attacks = InitAttacks();
	printf("Initialized %d attacks.\n", Attacks);
	int Turn = 0;


	BUFHANDLE b = CreateBuffer();
	SwitchBuffer(b);

	while (IsGameOngoing()) {
		Round = (Turn / PlayerCount) + 1;
		int CurrentPlayerID = Turn % EntityCount;

		Entity_t *CurrentPlayer = &Entities[CurrentPlayerID];

		if (IsEntityAlive(CurrentPlayer)) {
			CurrentPlayer->TakeTurn(CurrentPlayer, Round);
			sleep(1500);
		}

		Turn++;
	}

	printf("Game over.\n");
}

// temporary implementation
int IsGameOngoing() {
	for (size_t i = 0; i < EntityCount; i++) {
		Entity_t *Entity = &Entities[i];

		if (!IsEntityAlive(Entity))
			continue;

		if (HasLivingEnemies(Entity))
			return 1;
	}

	return 0;
}
