#include<stdio.h>
#include"entity.h"
#include"rng.h"
#include"ui/console_manager/buffer.h"
//#include"ui/console_manager/console.h"

int IsGameOngoing();

const int PlayerCount = 3;
const int BotCount = 1;

const int ParticipantCount = PlayerCount + BotCount;

int Turn = 0;

void InitPlayers();
int InitAttacks();

Entity_t Players;

int main() {
	printf("Starting AutoBossfight: ReMatched\n");

	int Attacks = InitAttacks();
	printf("Initialized %d attacks.\n", Attacks);

	BUFHANDLE Buffer = CreateBuffer();

	ActiveBuffer = Buffer;

	bprintf(Buffer, "Test");

	int Round = 0;

	while (IsGameOngoing()) {
		int CurrentPlayerID = Turn % ParticipantCount;
		
		if (CurrentPlayerID == ParticipantCount) {
			// boss plays
		} else {
			
		}
	}
}

// temporary implementation
int IsGameOngoing() {
	return 1;
}
