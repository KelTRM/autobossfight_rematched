#pragma once

#include"console_manager/console.h"
#include"../entity.h"

int AskAttack(Entity_t CurrentPlayer, uint64_t Round);
int RefreshScoreboard(int PreserveCursorPosition);
void Prompt(const char *Prompt, char **Result, int EchoToBuffer);
