#include<stdio.h>
#include<stdlib.h>
#include"../entity.h"
#include"../utils/sleep.h"
#include"../attack_manager.h"
#include"color/color.h"
#include "console_manager/console.h"
#include"ui.h"

extern int Round;

#define ATTACK_DISPLAY_FORMAT		"To use a %s, use %zu"

void PrintAttack(Attack_t *Attack, AttackID_t AttackID);

int AskAttack(Entity_t *CurrentPlayer, uint64_t Round) {
	ClearScreen();
	RefreshScoreboard(0);

	Attack_t *CurrentAttack;
	size_t AttackIndex = 0;

	printf("\n");

	while ((CurrentAttack = GetAttackAtIndex(AttackIndex++)) != NULL) {
//		printf(ATTACK_DISPLAY_FORMAT, CurrentAttack->AttackName, AttackIndex);

		PrintAttack(CurrentAttack, AttackIndex);
//		if (CurrentAttack->FirstAvailableRound > 0) {
//			GetTerminalForegroundColorStr(100, 100, 100);
//			printf(ATTACK_DISPLAY_FORMAT " (Available round %d+)",
//					CurrentAttack->AttackName, AttackIndex,
//					CurrentAttack->FirstAvailableRound);
//			ResetTerminalForegroundColorStr();
//		} else {
//			printf(ATTACK_DISPLAY_FORMAT, CurrentAttack->AttackName, AttackIndex);
//		}

//		printf("\n");
	}

	printf("CurrentPlayer = %p, CurrentPlayer.Name = %s", CurrentPlayer, CurrentPlayer->Name);
//	printf("CurrentPlayer.Name = %s", CurrentPlayer->Name);
	printf("\nCurrent round: %d\n\nIt's currently %s's turn.\n", Round, CurrentPlayer->Name);

	if (GetEnemyAtIndex(CurrentPlayer, 0) == NULL) {
		// easter egg
		printf("%s is a pacifist.\n", CurrentPlayer->Name);
		return 0;
	}

	AttackID_t ChosenAttack = (AttackID_t)-1;	

	while (ChosenAttack == (AttackID_t)-1) {
		BUFHANDLE b = CreateBuffer();
		CopyBuffer(INVALID_BUFFER_HANDLE, b);

		char *Result;
		Prompt("Which attack are you gonna use?", &Result, 0);

		CopyBuffer(b, INVALID_BUFFER_HANDLE);
		//RefreshScreen();

		DeleteBuffer(b);

		if (Result == NULL) {
			printf("[ERROR] Failed to prompt.\n");
			break;
		}

		AttackID_t AttackID = atoi(Result);
//		printf("AttackID - %d\n", AttackID);
		if (GetAttackAtIndex(AttackID-1) == NULL || AttackID == 0) {
			printf("Invalid Attack '%s'. Please choose a valid attack.\n", Result);
			sleep(1000);
		
			continue;
		}
		ChosenAttack = AttackID-1;
		//ChosenAttack = AttackID-1;
	}

	Attack_t *TargetAttack = GetAttackAtIndex(ChosenAttack);

	AttackData_t Attack = AttackEntity(TargetAttack, GetEnemyAtIndex(CurrentPlayer, 0), CurrentPlayer);
//	AttackData_t Result = TargetAttack->Attack(GetEnemyAtIndex(CurrentPlayer, 0), CurrentPlayer);

//	TargetAttack->Announcer(&);

	// sleep(2500);
	//printf("Not implemented.\n");
	return 0;
}

void PrintAttack(Attack_t *Attack, AttackID_t AttackID) {
	if (Attack->FirstAvailableRound > 0) {
		GetTerminalForegroundColorStr(100, 100, 100);
		printf(ATTACK_DISPLAY_FORMAT " (Available round %d+)",
				Attack->AttackName, AttackID,
				Attack->FirstAvailableRound);
		ResetTerminalForegroundColorStr();
	} else {
		printf(ATTACK_DISPLAY_FORMAT " (Requires at least %d%% energy)", Attack->AttackName, AttackID, Attack->MinimumEnergy);
	}

	printf("\n");
}
