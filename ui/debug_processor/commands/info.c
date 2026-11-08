#include<string.h>
#include<limits.h>
#include"../debug_processor.h"
#include"../attacks/attack_manager.h"

int GetInfo(int argc, const char **argv) {
	if (ParamRange(argc, argv, 2, INT_MAX))
		return 1;

	AttackIter_t AttackIter = OpenAttackIterator();


	Attack_t *Attack;
	while ((Attack = StepAttackIterator(&AttackIter)) != NULL) {
		if (Attack->Identifier == NULL) {
			printf("Warning: Attack '%s' has identifier of NULL. This attack cannot be searched unless identifier is set.\n", Attack->AttackName);
			continue;
		}
		if (strcmp(Attack->Identifier, argv[1]) == 0) {
			break;
		}
	}

	if (Attack == NULL) {
		printf("Couldn't find attack %s.\n", argv[1]);
		return 1;
	}

	printf("%s - %s\n", Attack->AttackName, Attack->Description);
	return 0;
}
