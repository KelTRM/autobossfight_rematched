#include"../entity/entity_manager.h"
#include<stdlib.h>
#include<string.h>

typedef struct DefinitionMgr {
	union {
		void *GenericDefinition;
		struct PlayerDefinition *PlayerDefinition;
		struct BossDefinition *BossDefinition;
	};

	size_t DefinitionCount;
} DefMgr_t;

int CreateDefMgr(DefMgr_t *mgr) {
	void *NewBuffer = malloc(1);
	if (NewBuffer == NULL)
		return 0;

	*mgr = (DefMgr_t){ 0 };
	mgr->GenericDefinition = NewBuffer;
	return 1;
}

int ValidateDefmgr(DefMgr_t *mgr) {
	if (mgr == NULL) return 0;
	if (mgr->GenericDefinition == NULL) return 0;
	
	return 1;
}

size_t ReallocateDefinitions(DefMgr_t *mgr, size_t Elements, size_t DefinitionSize) {
	size_t RequestedBytes = Elements * DefinitionSize;
	void *New = realloc(mgr->GenericDefinition, RequestedBytes);

	if (New == NULL)
		return 0;

	mgr->GenericDefinition = New;
	mgr->DefinitionCount = Elements;

	return RequestedBytes;
}

size_t AddPlayer(DefMgr_t *mgr, struct PlayerDefinition *Player) {
	if (ValidateDefmgr(mgr) == 0)
		return 0;

	size_t NewPlayerCount = mgr->DefinitionCount+1;

	size_t Bytes = ReallocateDefinitions(
		mgr,
		NewPlayerCount,
		sizeof(*Player)
	);

	if (Bytes == 0) return 0;
	
	memcpy(
		&mgr->PlayerDefinition[mgr->DefinitionCount-1],
		Player, sizeof(*Player)
	);

	return 1;
}

size_t AddBoss(DefMgr_t *mgr, struct BossDefinition *Boss) {
	if (ValidateDefmgr(mgr) == 0)
		return 0;
	
	size_t NewBossCount = mgr->DefinitionCount+1;

	size_t Bytes = ReallocateDefinitions(
		mgr,
		NewBossCount,
		sizeof(*Boss)
	);

	if (Bytes == 0) return 0;
	
	memcpy(
		&mgr->BossDefinition[mgr->DefinitionCount-1],
		Boss, sizeof(*Boss)
	);

	return 1;
}

