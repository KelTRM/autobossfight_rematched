#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include"entity.h"
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
size_t InitTransformations(void *Lua);

void *InitLua(void);
void InitEntities(void *Lua);

extern Entity_t *Entities;
extern size_t EntityCount;

void ANSI_SaveCurrentConsoleBuffer(void);
void ANSI_RestorePreviousConsoleBuffer(void);
void OnSigInt(int);

int main(void) {
	ANSI_SaveCurrentConsoleBuffer();

	BUFHANDLE b = CreateBuffer();
	SwitchBuffer(b);

	init_debug();
	define_debug_flush_location(fopen(LOGFILE_LOCATION, "w"));

	signal(SIGINT, OnSigInt);

	// should initialize the lua state
	void *Lua = InitLua();

	Init();
	InitEntities(Lua);

	int Attacks = InitAttacks(Lua);
	printf("Initialized %d attacks.\n", Attacks);

	int Transformations = InitTransformations(Lua);
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
	sleep(2500);
	ANSI_RestorePreviousConsoleBuffer();
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

void OnSigInt(int Signal) {
	(void)Signal;
	write_log(Info, "Recieved signal. Exiting...");
	ANSI_RestorePreviousConsoleBuffer();
	exit(0);
}

void ANSI_SaveCurrentConsoleBuffer(void) {
	fprintf(stdout, "\x1b[?1049h");
}

void ANSI_RestorePreviousConsoleBuffer(void) {
	fprintf(stdout, "\x1b[?1049l");
}
