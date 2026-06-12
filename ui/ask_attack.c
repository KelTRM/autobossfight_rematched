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
			printf(" (Available round %d+)", CurrentAttack->FirstAvailableRound);
		}

		printf("\n");
	}

	sleep(2500);
	//printf("Not implemented.\n");
	return 0;
}
