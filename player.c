#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include"entity.h"
#include"ui/ui.h"
#include"ui/strings.h"
#include"ui/color/color.h"

const Health_t PlayerAttack = 20000;
const Energy_t StartingEnergy = 100;

extern Entity_t *Entities;
extern size_t EntityCount;

static int PlayerDisplay(Entity_t *Self, size_t HealthPadding, size_t NamePadding) {
	size_t PrintedChars = 0;

	char *NameStr = PadRight(Self->Name, NamePadding, ' ');
	char *HpStr = IntToStr(Self->HealthPoints);

	char *HealthStr = PadLeft(HpStr, HealthPadding, ' ');
	free(HpStr);

	PrintedChars += printf("%s has %s hp [", NameStr, HealthStr);

	GetTerminalForegroundColorStr(0, 100, 255);
	for (int j = 0; j < MAX_ENERGY; j += MAX_ENERGY / ENERGY_DISP_PRECISION) {
		if (Self->Energy > j)		PrintedChars += printf("%s", BOX_CHAR);
		else				PrintedChars += printf(" ");
	}
	ResetTerminalForegroundColorStr();
	PrintedChars += printf("] (%d%%) [PLAYER]\n", Self->Energy);

	free(NameStr);
	free(HealthStr);

	return PrintedChars;
}

Entity_t CreatePlayer(const char *Name, Health_t MaxHP, Health_t MinHeal, Health_t MaxHeal, Color_t PlayerColor) {
	Entity_t e = { };

	e.Attack = PlayerAttack;
	e.Energy = StartingEnergy;

	e.HealthPoints = MaxHP;
	e.HealingMinimum = MinHeal;
	e.HealingMaximum = MaxHeal;

	e.Name = Name;
	e.Enemies = NULL;

	e.TakeTurn = AskAttack;
	e.DisplayEntity = PlayerDisplay;

	e.AI_RechargeEnergy = 0;

	e.Type = ENTITY_TYPE_PLAYER;
	e.EntityColor = PlayerColor;

	return e;
}
