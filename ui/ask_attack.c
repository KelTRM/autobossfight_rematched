#include<stdio.h>
#include"../entity.h"
#include"console_manager/console.h"
#include"../utils/sleep.h"
#include"../attack_manager.h"

int RefreshScoreboard(int PreserveCursorPosition);

int AskAttack(Entity_t CurrentPlayer, uint64_t Round) {
	ClearScreen();
	RefreshScoreboard(0);

	Attack_t *CurrentAttack;
	size_t AttackIndex = 0;

	printf("\n");

	while ((CurrentAttack = GetAttackAtIndex(AttackIndex++)) != NULL) {
		printf("To use a %s, use %zu", CurrentAttack->AttackName, AttackIndex);

		if (CurrentAttack->FirstAvailableRound > 0) {
			// separate printf as to allow an if statement in the future
			printf(" (Available round %d+)", CurrentAttack->FirstAvailableRound);
		}

		printf("\n");

//		++AttackIndex;
	}

	sleep(2500);
	//printf("Not implemented.\n");
	return 0;
}
