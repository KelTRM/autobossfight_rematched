//#include"linked_list/list.h"
#include"registration.h"
#include<stddef.h>
#include<stdlib.h>
#include<string.h>
//#include<ui.h>

// How many attacks can be registered at once
#define REGISTRAR_DEFAULT_LIMIT	256

size_t InitRegistrar(size_t MaxRegistrations, Registrar_t *Registrar) {
	// Can't put it nowhere
	if (Registrar == NULL)
		return 0;

	// Get the registrar's buffer
	if (MaxRegistrations == 0)
		MaxRegistrations = REGISTRAR_DEFAULT_LIMIT;
	size_t RegistrarBufferSize = sizeof(void*) * MaxRegistrations;

	Registrar->RegistrationMap = malloc(RegistrarBufferSize);

	// safety checks
	if (Registrar->RegistrationMap == NULL) {
		return 0;
	}

	// Set data in the registrar itself
	Registrar->MaxRegistrations = MaxRegistrations;
	Registrar->RegistrationList = NULL;

	// null out the hashmap	
	memset(Registrar->RegistrationMap, 0, RegistrarBufferSize);

	return MaxRegistrations;
}

int RegistrarAdd(Registrar_t *Registrar, void *Registree, RegistreeID_t RegistreeID) {
	if (Registrar == NULL) return 0;	// Don't add a registree if it's NULL

	// Don't add if it conflicts with another of the same ID
	if (Registrar->RegistrationMap[RegistreeID] != NULL) {
		return 0;
	}

	// Check if the ID is out of bounds
	if (RegistreeID >= Registrar->MaxRegistrations) {
		return 0;
	}

	Registrar->RegistrationCount++;
	Registrar->RegistrationMap[RegistreeID] = Registree;

	return 1;
}

size_t BuildRegistrationList(Registrar_t *Registrar) {
	// TODO - modify existing list instead of deleting it
	if (Registrar->RegistrationList != NULL)
		free(Registrar->RegistrationList);

	// search registration map
	size_t RegistrationsFound = 0;
	Linked_t *Node = &Registrar->RegistrationList;

	for (RegistreeID_t ID = 0; ID < Registrar->MaxRegistrations; ID++) {
		void *Registration = Registrar->RegistrationMap[ID];

		// NULL registration is unused. ignore it
		if (Registration == NULL)
			continue;

		// define the node
		*Node = malloc(sizeof(struct List));
		memset(*Node, 0, sizeof(struct List));

		// Set value to current registration, step into the next node
		(*Node)->Value = Registration;
		Node = &(*Node)->Next;

		RegistrationsFound++;
	}

	return RegistrationsFound;
}

RegistrationIter_t OpenRegistrationIterator(Registrar_t *Registrar) {
	return (RegistrationIter_t)Registrar->RegistrationList;
}

void *StepRegistrationIterator(RegistrationIter_t *Iterator) {
	Linked_t Node = *((Linked_t*)Iterator);
	void *CurrentRegistration = (void*)Node->Value;

	*Iterator = Node->Next;

	return CurrentRegistration;
}

void *GetRegistrationFromID(Registrar_t *Registrar, RegistreeID_t Index) {
	if (Index > Registrar->MaxRegistrations) return NULL;
	return Registrar->RegistrationMap[Index];
}
