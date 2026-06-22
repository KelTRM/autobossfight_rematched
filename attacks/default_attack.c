#include<stdlib.h>
#include"../attack.h"
#include"../ui/strings.h"

void DefaultAnnouncer(AttackData_t *Attack) {
	char *str = IntToStr(Attack->Damage);
	printf("%s dealt %s damage to %s\n",
			Attack->Attacker->Name, str, Attack->Target->Name);
	free(str);
}
