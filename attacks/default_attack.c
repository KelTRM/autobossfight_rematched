#include"../attack.h"
#include"../ui/console_manager/console.h"

void DefaultAnnouncer(AttackData_t *Attack) {
	printf("%s dealt %llu damage to %s\n",
			Attack->Attacker->Name, Attack->Attacker->Attack, Attack->Target->Name);
}
