#pragma once

#include"attack.h"
#include<stddef.h>

typedef void	*AttackIter_t;

int InitAttackRegistrar(void);
int RegisterAttack(Attack_t *Attack);
Attack_t *GetAttackAtIndex(size_t Index);
Entity_t **GetApplicableEntities(Attack_t *AttemptingAttack, Entity_t *Attacker, size_t *ApplicableCount);

size_t BuildAttackList(void);
AttackIter_t OpenAttackIterator(void);
Attack_t *StepAttackIterator(AttackIter_t *Iterator);

Attack_t *GetAttackFromID_str(const char *id);
Attack_t *GetAttackFromID_num(AttackID_t id);

#define GetAttackFromID(id)	_Generic((id),		\
	const char*:	GetAttackFromID_str(id),	\
	AttackID_t:	GetAttackFromID_num(id)		\
)
