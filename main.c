#include<stdio.h>
#include"rng.h"

int IsGameOngoing();

const int PlayerCount = 3;
const int BotCount = 1;

const int ParticipantCount = PlayerCount + BotCount;

int Turn = 0;

int main() {
	printf("Starting AutoBossfight: ReMatched\n");

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
