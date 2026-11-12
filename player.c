#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<alloca.h>
#include"entity.h"
#include"transformation.h"
#include"ui/ui.h"
#include"ui/strings.h"
#include"ui/color/color.h"

const Health_t PlayerAttack = 20000;
const Energy_t StartingEnergy = 100;

extern Entity_t *Entities;
extern size_t EntityCount;

static int PlayerDisplay(BUFHANDLE Where,
			Entity_t *Self,
			int ShowEnergy,
			size_t HealthPadding,
			size_t NamePadding,
			size_t FormPadding) {
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

	const char *FormString = GetTransformationNameStr(Self->EntityTransformation);
	const char *RegularStr = Self->EntityTransformation->TransformationName;

	size_t PaddingAmount = (strlen(FormString) - strlen(RegularStr)) + FormPadding;

	char *str = alloca(PaddingAmount + 3);
	snprintf(str, PaddingAmount+3, "[%s]", FormString);

	char *FormStr = PadLeft(str, FormPadding+2, ' ');

	char *progressbar = ProgressBar(Self->Energy, ENERGY_DISP_PRECISION, (Color_t){
		.r=0,
		.g=100,
		.b=255
	});

	PrintedChars += bprintf(Where, "%s %s has %s hp",
				NameStr,
				FormStr,
				HealthStr);

	if (ShowEnergy)
		PrintedChars += bprintf(Where, " %s (%d%%)", progressbar, Self->Energy);

	PrintedChars += bprintf(Where, "\n");
	free(progressbar);
	free(NameStr);
	free(FormStr);
	free(HealthStr);

	return PrintedChars;
}

Entity_t CreatePlayer(const char *Name, Health_t MaxHP, Health_t MinHeal, Health_t MaxHeal, Color_t PlayerColor) {
	Entity_t e = { 0 };

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

	e.AttackMultiplier = ATTACK_MULTIPLIER_BASE;

	e.EntityTransformation = (Transformation_t*)&BaseTransformation;

	return e;
}
