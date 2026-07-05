#include "attack.h"

AttackData_t AttackEntity(Attack_t *Attack, Entity_t *Target, Entity_t *Attacker) {
	AttackData_t Result = Attack->Attack(Attack, Target, Attacker);
	Result.Announcer(&Result);
	return Result;
}
