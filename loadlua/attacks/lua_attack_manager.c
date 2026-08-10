#include<stdlib.h>
#include"../../attacks/attack.h"

// # of IDs allocated at a time
#define ATTACK_BLOCK_SIZE		10
#define MAX_ATTACKS_DEFAULT		65535

typedef struct AttackPlugin {
	Attack_t *Attacks;
	size_t Count;
} AttackPlugin_t;

typedef struct AttackManager {
	struct Attacks {
		Attack_t *Attack[ATTACK_BLOCK_SIZE];
		uint32_t PluginID;
	} *Attacks;

	size_t MaxAttackCount;
} AttackMgr_t;

/*
 * --- PLUGIN ID MODEL ---
 *
 *  Attack IDs will be allocated within the maximum attack count.
 *  the IDs will shall be allocated in a first-come first-serve manner by plugins
 *  The allocated section of IDs will act as a virtual address space within the plugin.
 *
 *  The plugins which specifically request an ID get priority, while those which request no specific ID
 *  will get lower priority. (If they're registered in order of { 0, 1, 0 }, they will get IDs of { 2, 1, 3 })
 */

// MaxAttacks = 0 gives default
AttackMgr_t OpenAttackAllocator(size_t MaxAttacks) {
	if (MaxAttacks == 0)
		MaxAttacks = MAX_ATTACKS_DEFAULT;

	AttackMgr_t Manager;
	Manager.Attacks = NULL;
	Manager.MaxAttackCount = (MaxAttacks / ATTACK_BLOCK_SIZE) + ((MaxAttacks % ATTACK_BLOCK_SIZE) > 0);

	return Manager;
}

void AllocateAttackPlugin(AttackMgr_t *Manager, size_t RequiredPlugins) {
	
}
