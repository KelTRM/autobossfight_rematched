#include"attack_manager.h"
#include<string.h>

#include"../debug/debug.h"

Attack_t *GetAttackFromID_str(const char *id) {
	if (id == NULL)
		return NULL;

	AttackIter_t Iterator = OpenAttackIterator();
	
	Attack_t *Attack;
	while ((Attack = StepAttackIterator(&Iterator)) != NULL) {
		if (Attack->Identifier == NULL)
			write_debug(GetAttackFromID, "note: attack %s missing identifier.",
					Attack->AttackName);
		int cmp = strcmp(Attack->Identifier, id);

		if (cmp == 0)
			break;
	}

	return Attack;
}

Attack_t *GetAttackFromID_num(AttackID_t id) {
	return GetAttackAtIndex(id);
}
