#pragma once

#include"linked_list/list.h"
#include<stdint.h>
#include<stddef.h>

typedef uint64_t RegistreeID_t;
typedef Linked_t RegistrationIter_t;

typedef struct Registrar {
	void **RegistrationMap;
	Linked_t RegistrationList;

	size_t MaxRegistrations;
	size_t RegistrationCount;
} Registrar_t;

size_t InitRegistrar(size_t MaxRegistrations, Registrar_t *Registrar);
int RegistrarAdd(Registrar_t *Registrar, void *Registree, RegistreeID_t RegistreeID);
size_t BuildRegistrationList(Registrar_t *Registrar);
RegistrationIter_t OpenAttackIterator(Registrar_t *Registrar);
void *StepAttackIterator(RegistrationIter_t *Iterator);
