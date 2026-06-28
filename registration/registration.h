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
