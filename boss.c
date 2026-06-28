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

#define AttackMultiplierPercent(multiplier)	\
	multiplier / (ATTACK_MULTIPLIER_MAX / 100)

int BossTurn(Entity_t *CurrentPlayer, uint64_t Round);

extern Entity_t *Entities;
extern size_t EntityCount;

static int BossDisplay(BUFHANDLE Where, Entity_t *Self, int ShowEnergy, size_t HealthPadding, size_t NamePadding);

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

	Boss.AttackMultiplier = ATTACK_MULTIPLIER_BASE;

	Boss.EntityTransformation = &BossTransformation;

	return Boss;
}

#define BOSS_ENERGY_RECHARGE_THRESHOLD	90
#define BOSS_ENERGY_RECHARGE_TO		95

const Attack_t *BossAI(Entity_t *Boss) {
	if (Boss->Energy < Boss->AI_RechargeEnergy)
		return &NothingAttack;

	Boss->AI_RechargeEnergy = 0;

	if (Boss->Energy < BOSS_ENERGY_RECHARGE_THRESHOLD) {
		Boss->AI_RechargeEnergy = BOSS_ENERGY_RECHARGE_TO;
	}

	return &NormalAttack;
}

int BossTurn(Entity_t *CurrentPlayer, uint64_t Round) {
	(void)Round; // get rid of unused parameter error, as it's a function pointer parameter used in another instance

	const Attack_t *ChosenAttack = BossAI(CurrentPlayer);

	Entity_t *Target = NULL;
	while (Target == NULL) {
		int EntityToAttack = GetRandomIntBetween(0, CurrentPlayer->EnemyCount);
		Entity_t *CurrentOption = GetEnemyAtIndex(CurrentPlayer, EntityToAttack);

		if (!IsEntityAlive(CurrentOption))
			continue;
		Target = CurrentOption;
	}

	AttackEntity((Attack_t*)ChosenAttack, Target, CurrentPlayer);

	// +20% attack multiplier
	CurrentPlayer->AttackMultiplier += ATTACK_MULTIPLIER_BASE / 5;
	return 0;
}

static int BossDisplay(BUFHANDLE Where, Entity_t *Self, int ShowEnergy, size_t HealthPadding, size_t NamePadding) {
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

	char *progressbar = ProgressBar(
		AttackMultiplierPercent(Self->AttackMultiplier),
		ENERGY_DISP_PRECISION,
		(Color_t){
			.r=150,
			.g=0,
			.b=0
		}
	);

	PrintedChars += bprintf(
		Where,
		"%s has %s hp",
		NameStr,
		HealthStr
	);

	if (ShowEnergy)
		PrintedChars += bprintf(
			Where,
			" %s (%llu.%dx)",
			progressbar,
			Self->AttackMultiplier / ATTACK_MULTIPLIER_BASE,
			Self->AttackMultiplier % ATTACK_MULTIPLIER_BASE
			//Self->AttackMultiplier / (ATTACK_MULTIPLIER_BASE / 100)
		);

	bprintf(Where, " [%s]\n", Self->EntityTransformation->Name);

	free(progressbar);
	free(NameStr);
	free(HealthStr);

	return PrintedChars;
}
