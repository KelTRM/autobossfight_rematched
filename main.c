#include<stdio.h>
#include"entity.h"
#include"rng.h"
//#include"ui/console_manager/buffer.h"
#include"ui/ui.h"
#include"utils/sleep.h"

//#ifdef __linux__
//#include<unistd.h>
//#endif

int IsGameOngoing();

const int PlayerCount = 3;
const int BotCount = 1;

const int ParticipantCount = PlayerCount + BotCount;

int Round = 0;

void InitPlayers();
int InitAttacks();

extern Entity_t Players[3];

int main() {
	InitPlayers();

	//printf("Starting AutoBossfight: ReMatched\n");

	int Attacks = InitAttacks();
	printf("Initialized %d attacks.\n", Attacks);
	int Turn = 0;

	BUFHANDLE b = CreateBuffer();
	SwitchBuffer(b);

	while (IsGameOngoing()) {
		int CurrentPlayerID = Turn % PlayerCount;//ParticipantCount;
		
		if (CurrentPlayerID == ParticipantCount) {
			// boss plays
		} else {
			AskAttack(&Players[CurrentPlayerID], 0);
		}

		Turn++;

		sleep(1500);
	}
}

// temporary implementation
int IsGameOngoing() {
	return 1;
}
