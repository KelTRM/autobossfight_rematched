#pragma once

#include"console_manager/console.h"
#include"color/color.h"
#include"../entity.h"

#define BOX_CHAR		"▅"
#define ENERGY_DISP_PRECISION	20

int AskAttack(Entity_t *CurrentPlayer, uint64_t Round);
int RefreshScoreboard(int PreserveCursorPosition);
void Prompt(const char *Prompt, char **Result, int EchoToBuffer);
char *ProgressBar(uint8_t Percentage, uint64_t Precision, Color_t ProgressBarColor);
