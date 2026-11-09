#include<stdlib.h>
#include"list.h"

int ListInsertAfter(Linked_t Node, void *Value) {
	Linked_t NewNode = malloc(sizeof(*NewNode));
	if (NewNode == NULL) return 0;

	NewNode->Next = Node->Next;
	NewNode->Value = Value;

	Node->Next = NewNode;

	return 1;
}

Linked_t ListGetIndex(Linked_t List, size_t Index) {
	size_t CurrentIndex = 0;
	while (CurrentIndex < Index) {
		if (List == NULL)
			return NULL;

		List = List->Next;
		CurrentIndex++;
	}

	return List;
}

void RemoveFollowingNode(Linked_t Node) {
	if (Node->Next == NULL)
		return;

	Linked_t ToRemove = Node->Next;
	Node->Next = ToRemove->Next;

	free(ToRemove);
}
