#pragma once

#include<stddef.h>
#include"../entity/entity_manager.h"

typedef struct DefinitionMgr {
	union {
		void *GenericDefinition;
		struct PlayerDefinition *PlayerDefinition;
		struct BossDefinition *BossDefinition;
	};

	size_t DefinitionCount;
} DefMgr_t;

int CreateDefMgr(DefMgr_t *mgr);
int ValidateDefmgr(DefMgr_t *mgr);
size_t ReallocateDefinitions(DefMgr_t *mgr, size_t Elements, size_t DefinitionSize);
size_t AddPlayer(DefMgr_t *mgr, struct PlayerDefinition *Player);
size_t AddBoss(DefMgr_t *mgr, struct BossDefinition *Boss);

