#include<stdlib.h>
#include"../../attacks/attack.h"

// # of IDs allocated at a time
#define ATTACK_BLOCK_SIZE		10
#define MAX_ATTACKS_DEFAULT		65535

typedef struct AttackPlugin {
	Attack_t *Attacks;
	size_t Count;
} AttackPlugin_t;

typedef struct LuaAttackManager {
	struct PluginAllocation {
		AttackPlugin_t *Plugins;
		size_t BlocksAlloc;
		size_t BlockOffset;
	} *Plugins;
	size_t PluginCount;
} LuaAttackMgr_t;

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
LuaAttackMgr_t OpenAttackAllocator(size_t MaxAttacks) {
	if (MaxAttacks == 0)
		MaxAttacks = MAX_ATTACKS_DEFAULT;

	LuaAttackMgr_t Manager;
	Manager.Plugins = NULL;
	Manager.PluginCount = 0;

	return Manager;
}
