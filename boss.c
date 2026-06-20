#include<stdlib.h>

#include"boss.h"
#include"attacks/attacks.h"
#include"attack_manager.h"
#include"entity.h"
#include"rng.h"

#include"ui/console_manager/console.h"
#include"ui/color/color.h"
#include"ui/strings.h"
#include"ui/ui.h"

int BossTurn(Entity_t *CurrentPlayer, uint64_t Round);

extern Entity_t *Entities;
extern size_t EntityCount;

static int BossDisplay(Entity_t *Self, size_t HealthPadding, size_t NamePadding);

Entity_t CreateBoss(const char *Name, Health_t HP) {
	Entity_t Boss;
	Boss.Name = Name;
	Boss.HealthPoints = HP;
	Boss.Energy = 100;

	Boss.EnemyCount = 0;
	Boss.Enemies = NULL;

	Boss.HealingMinimum = 0;
	Boss.HealingMaximum = 0;

	Boss.Attack = 10000;

	Boss.TakeTurn = BossTurn;
	Boss.DisplayEntity = BossDisplay;

	Boss.AI_RechargeEnergy = 0;

	Boss.Type = ENTITY_TYPE_BOSS;
	Boss.EntityColor = GetColorFromHex(0x646464);

	return Boss;
}

#define BOSS_ENERGY_RECHARGE_THRESHOLD	90
#define BOSS_ENERGY_RECHARGE_TO		95

const Attack_t *BossAI(Entity_t *Boss) {
	if (Boss->Energy < BOSS_ENERGY_RECHARGE_THRESHOLD)
		Boss->AI_RechargeEnergy = BOSS_ENERGY_RECHARGE_TO;

	if (Boss->Energy < Boss->AI_RechargeEnergy) {
		return &NothingAttack;
	}

	return &NormalAttack;
}

int BossTurn(Entity_t *CurrentPlayer, uint64_t Round) {
	const Attack_t *ChosenAttack = BossAI(CurrentPlayer);

	int EntityToAttack = GetRandomIntBetween(0, CurrentPlayer->EnemyCount);
	AttackEntity((Attack_t*)ChosenAttack, GetEnemyAtIndex(CurrentPlayer, EntityToAttack), CurrentPlayer);
	return 0;
}

static int BossDisplay(Entity_t *Self, size_t HealthPadding, size_t NamePadding) {
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

	PrintedChars += printf("%s has %s hp %s (%d%%) [BOSS]\n",
			NameStr,
			HealthStr,
			progressbar,
			Self->Energy);
//	PrintedChars += printf(" (%d%%) [BOSS]\n", Self->Energy);

	free(progressbar);
	free(NameStr);
	free(HealthStr);

	return PrintedChars;
}
