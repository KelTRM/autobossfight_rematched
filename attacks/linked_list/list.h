#pragma once

#include<stddef.h>

struct List {
	struct List *Next;
	void *Value;
};

typedef struct List *Linked_t;

void ListInsertAfter(Linked_t Node, void *Value);
Linked_t ListGetIndex(Linked_t List, size_t Index);
