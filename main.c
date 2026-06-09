#include<stdio.h>
#include"entity.h"
#include"rng.h"
//#include"ui/console_manager/buffer.h"
#include"ui/console_manager/console.h"

//#ifdef __linux__
//#include<unistd.h>
//#endif

int IsGameOngoing();

const int PlayerCount = 3;
const int BotCount = 1;

const int ParticipantCount = PlayerCount + BotCount;

int Turn = 0;

void InitPlayers();
int InitAttacks();

extern Entity_t Players[3];

int main() {
	// i seriously hope this works...
	// OH MY FUCKING GOD IT WORKS
	printf("▅");

	printf("Starting AutoBossfight: ReMatched\n");

	int Attacks = InitAttacks();
	printf("Initialized %d attacks.\n", Attacks);
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
