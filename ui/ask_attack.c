#include<stdio.h>
#include"../entity.h"
#include"console_manager/console.h"

int RefreshScoreboard(int PreserveCursorPosition);

int AskAttack(Entity_t CurrentPlayer, uint64_t Round) {
	ClearScreen();
	RefreshScoreboard(0);
	printf("Not implemented.\n");
	return 0;
}
