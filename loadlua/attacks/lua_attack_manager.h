#pragma once

#include<stdint.h>
#include"../../attacks/attack.h"

typedef uint32_t	PluginID_t;
typedef int64_t		BlockID_t;

#define ATTACK_BLOCK_SIZE	10						// amount of ids allocated at a time
#define MAX_ATTACKS_DEFAULT	65535						// # of IDs allocated at a time
#define MAX_PLUGINS		((MAX_ATTACKS_DEFAULT / ATTACK_BLOCK_SIZE) + 1)	// amount of plugins allocated

#define PLUGIN_DEFINED		0xFFFFFFFFul					// Registered value for valid plugins
#define INVALID_PLUGIN_ID	0xFFFFFFFFul					// ID for invalid plugin

// plugin id refers to index into array
struct PluginRegistry {
	uint32_t Registered;		// 0xFFFFFFFF if registered, anything else otherwise
	
	BlockID_t FirstRegisteredBlock;
	size_t AllocatedBlockCount;

	// should instead check if existing plugin has ID=0, and move if true
//	// list of attacks with no specified allocation
//	Attack_t **UnallocatedAttacks;
//	size_t UnallocatedAttackCount;

	size_t MaxAttacks;
};

typedef struct AttackManager {
	struct Attacks {
		Attack_t *Attack[ATTACK_BLOCK_SIZE];
		uint32_t PluginID;
	} *Attacks;

	BlockID_t BlockIdCount;
	size_t MaxAttackCount;
	struct PluginRegistry Plugins[MAX_PLUGINS];
} AttackMgr_t;
