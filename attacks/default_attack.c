#include"../attack.h"
#include"../ui/console_manager/console.h"

void DefaultAnnounceAttack(AttackData_t *Attack) {
	printf("%s dealt %llu damage to %s", Attack->Attacker->Name, Attack->Attacker->Attack, Attack->Target->Name);
}
