#include"attacks.h"

void GenAttackData(Entity_t *From, Entity_t *To, Attack_t *What,
		Health_t Damage, Health_t Prior, AttackData_t *Attack) {
	if (Attack == NULL)
		return;

	Attack->Attacker = From;
	Attack->Target = To;
	Attack->Attack = What == NULL ? (AttackID_t)-1 : What->ID;
	Attack->Damage = Damage;
	Attack->PriorHealth = Prior;
	Attack->Announcer = DefaultAnnouncer;
}
