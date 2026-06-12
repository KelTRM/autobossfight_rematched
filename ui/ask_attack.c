#include<stdio.h>
#include"../entity.h"
#include"console_manager/console.h"
#include"../utils/sleep.h"
#include"../attack_manager.h"
#include"color/color.h"

#define ATTACK_DISPLAY_FORMAT		"To use a %s, use %zu"

int RefreshScoreboard(int PreserveCursorPosition);

int AskAttack(Entity_t CurrentPlayer, uint64_t Round) {
	ClearScreen();
	RefreshScoreboard(0);

	Attack_t *CurrentAttack;
	size_t AttackIndex = 0;

	printf("\n");

	while ((CurrentAttack = GetAttackAtIndex(AttackIndex++)) != NULL) {
//		printf(ATTACK_DISPLAY_FORMAT, CurrentAttack->AttackName, AttackIndex);

		if (CurrentAttack->FirstAvailableRound > 0) {
			GetTerminalForegroundColorStr(100, 100, 100);
			printf(ATTACK_DISPLAY_FORMAT " (Available round %d+)",
					CurrentAttack->AttackName, AttackIndex,
					CurrentAttack->FirstAvailableRound);
			ResetTerminalForegroundColorStr();
		} else {
			printf(ATTACK_DISPLAY_FORMAT, CurrentAttack->AttackName, AttackIndex);
		}

		printf("\n");
	}

	AttackID_t ChosenAttack = (AttackID_t)-1;

	while (true) {

	}

	//sleep(2500);
	//printf("Not implemented.\n");
	return 0;
}
