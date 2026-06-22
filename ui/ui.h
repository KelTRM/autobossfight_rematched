#pragma once

#include"console_manager/console.h"
#include"color/color.h"
#include"../entity.h"

#define BOX_CHAR		"▅"
#define ENERGY_DISP_PRECISION	20

#ifndef	USE_STD_STRLEN
#define strlen(str)	UTF8_Strlen(str)
#endif

int AskAttack(Entity_t *CurrentPlayer, uint64_t Round);
int RefreshScoreboard(int PreserveCursorPosition);
void Prompt(const char *Prompt, char **Result, int EchoToBuffer);
char *ProgressBar(uint8_t Percentage, uint64_t Precision, Color_t ProgressBarColor);
size_t UTF8_Strlen(const char *str);
Entity_t *SelectEntity(Entity_t **Entities, size_t EntityCount);
