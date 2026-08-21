#include "attack.h"
#include "attacks.h"

AttackData_t AttackEntity(Attack_t *Attack, Entity_t *Target, Entity_t *Attacker) {
	AttackData_t Result = Attack->Attack(Attack, Target, Attacker);
	if (Result.Announcer != NULL)
		Result.Announcer(&Result);
	else DefaultAnnouncer(&Result);
	return Result;
}
