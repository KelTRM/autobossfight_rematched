#include<stdio.h>
#include"entity.h"
#include"rng.h"
//#include"ui/console_manager/buffer.h"
#include"ui/console_manager/console.h"

#ifdef __linux__
#include<unistd.h>
#endif

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

	SwitchBuffer(Buffer);
//	ActiveBuffer = Buffer;

	bprintf(Buffer, "Test1\n");
	bprintf(Buffer, "Test2\n");

	sleep(3);

	BUFHANDLE Buffer2 = CreateBuffer();

	bprintf(Buffer2, "Tst1\n");
	bprintf(Buffer2, "Tst2\n");

	bprintf(Buffer2, "Tst3\n");

	SwitchBuffer(Buffer2);

	sleep(3);

	SwitchBuffer(Buffer);

	sleep(3);

	SwitchBuffer(Buffer2);

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
