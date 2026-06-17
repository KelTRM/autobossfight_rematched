#include<stdio.h>
#include"entity.h"
#include<stdint.h>
#include"ui/ui.h"

const Health_t PlayerAttack = 20000;
const Energy_t StartingEnergy = 100;

Entity_t CreatePlayer(const char *Name, Health_t MaxHP, Health_t MinHeal, Health_t MaxHeal) {
	Entity_t e = { };

	e.Attack = PlayerAttack;
	e.Energy = StartingEnergy;

	e.HealthPoints = MaxHP;
	e.HealingMinimum = MinHeal;
	e.HealingMaximum = MaxHeal;

	e.Name = Name;

	// NULL if enemy doesn't exist yet
	e.Enemies = NULL;

	e.TakeTurn = AskAttack;

	e.AI_RechargeEnergy = 0;

	return e;
}
