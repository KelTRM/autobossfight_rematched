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

static int PlayerDisplay(BUFHANDLE Where, Entity_t *Self, int ShowEnergy, size_t HealthPadding, size_t NamePadding) {
	size_t PrintedChars = 0;

	char *NameColoredStr = GetEntityNameStr(Self);
	
	size_t ColoredLen = strlen(NameColoredStr);
	size_t NormalLen = strlen(Self->Name);

	size_t ColoredAdditionalChars = ColoredLen - NormalLen;

	char *NameStr = PadRight(NameColoredStr, NamePadding + ColoredAdditionalChars, ' ');
	char *HpStr = IntToStr(Self->HealthPoints);

	free(NameColoredStr);

	char *HealthStr = PadLeft(HpStr, HealthPadding, ' ');
	free(HpStr);

	char *progressbar = ProgressBar(Self->Energy, ENERGY_DISP_PRECISION, (Color_t){
		.r=0,
		.g=100,
		.b=255
	});

	PrintedChars += bprintf(Where, "%s has %s hp",
				NameStr,
				HealthStr);

	if (ShowEnergy)
		PrintedChars += bprintf(Where, " %s (%d%%)", progressbar, Self->Energy);

	PrintedChars += bprintf(Where, " [PLAYER]\n");
	
	free(progressbar);
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
