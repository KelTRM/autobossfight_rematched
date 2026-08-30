#include<assert.h>
#include<stdlib.h>
#include<string.h>
#include"lua_attack_manager.h"
#include"../../registration/registration.h"
#include"../../attacks/attack.h"
#include"../../debug/debug.h"

typedef uint32_t	PluginID_t;
typedef int64_t		BlockID_t;

// todos:
// implement plugin handle system - done
// implement plugin attack size handler - done
// implement interface for adding/removing plugin attacks - half done

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
	Manager.MaxAttackCount = ((MaxAttacks-1) / ATTACK_BLOCK_SIZE)+1;
	Manager.Attacks = calloc(Manager.MaxAttackCount, sizeof(struct Attacks));

	Manager.BlockIdCount = Manager.MaxAttackCount / ATTACK_BLOCK_SIZE;

	if (Manager.Attacks == NULL)
		return (AttackMgr_t){ 0 };

	for (size_t i = 0; i < Manager.MaxAttackCount; i++) {
		memset(Manager.Attacks[i].Attack, 0, sizeof(Attack_t*) * ATTACK_BLOCK_SIZE);
		Manager.Attacks[i].PluginID = -1;
	}

	return Manager;
}

size_t GetPluginSizeFromID(AttackMgr_t *mgr, PluginID_t ID) {
	if (mgr->Plugins[ID].Registered != PLUGIN_DEFINED) return 0;
	return mgr->Plugins[ID].MaxAttacks;
//	return 0;
}

static PluginID_t GetNewPluginID(AttackMgr_t *mgr, BlockID_t FirstBlock, size_t BlockCount, size_t AttackCount) {
	for (PluginID_t i = 0; i < MAX_PLUGINS; i++) {
		if (mgr->Plugins[i].Registered == PLUGIN_DEFINED) continue;

		// PluginID = i
		mgr->Plugins[i].Registered = PLUGIN_DEFINED;
		mgr->Plugins[i].MaxAttacks = AttackCount;
		mgr->Plugins[i].FirstRegisteredBlock = FirstBlock;
		mgr->Plugins[i].AllocatedBlockCount = BlockCount;
		return i;
	}
	return INVALID_PLUGIN_ID;
}

int ValidatePlugin(AttackMgr_t *mgr, PluginID_t ID) {
	if (ID >= MAX_PLUGINS) return 0;
	if (mgr == NULL) return 0;
	if (mgr->Plugins[ID].Registered != PLUGIN_DEFINED) return 0;

	return 1;
}

Attack_t **IndexPluginSpace(AttackMgr_t *mgr, PluginID_t ID, AttackID_t Attack) {
	if (ValidatePlugin(mgr, ID) == 0) return NULL;

	size_t MaxAttacks = mgr->Plugins[ID].MaxAttacks;
	if (Attack > MaxAttacks) return NULL;

	BlockID_t PluginBlock = mgr->Plugins[ID].FirstRegisteredBlock;

	BlockID_t IndexBlock = (Attack / ATTACK_BLOCK_SIZE) + PluginBlock;
	if (IndexBlock > mgr->BlockIdCount) return NULL;

	uint32_t BlockIndex = Attack % ATTACK_BLOCK_SIZE;
	
	write_debug(IndexPluginSpace, "Indexing @ ID=%d & idx=%d (block=%d,idx=%d)",
			ID, Attack, IndexBlock, BlockIndex);

	return &mgr->Attacks[IndexBlock].Attack[BlockIndex];
}

size_t AddAttackToPlugin(AttackMgr_t *mgr, PluginID_t ID, Attack_t *Attack) {
	// make sure the plugin exists
	if (ValidatePlugin(mgr, ID) == 0) return 0;

	// confirm attack exists
	if (Attack == NULL)
		return 0;

	AttackID_t RequestedID = Attack->ID;

	if (Attack->ID != 0) {
		Attack_t **Requested = IndexPluginSpace(mgr, ID, Attack->ID);
		write_debug(AddAttackToPlugin, "Requested = %p", Requested);
		if (Requested == NULL)
			goto unallocated;

		Attack_t *ExistingAttack = *Requested;
		if (ExistingAttack != NULL) {
			write_debug(AddAttackToPlugin, "ExistingAttack->ID = %d",
					ExistingAttack->ID);
			if (ExistingAttack->ID == RequestedID) {
				// requested ID taken. go after different spot
				goto unallocated;
			}

			*Requested = NULL;

			// a little recursion never hurts
			AddAttackToPlugin(mgr, ID, Attack);
			AddAttackToPlugin(mgr, ID, ExistingAttack);

			write_debug(AddAttackToPlugin, "*Requested = %p; Attack = %p",
					*Requested, Attack);
		}

		write_debug(AddAttackToPlugin, "Assigning attack %s to ID %d (req=%d)",
				Attack->AttackName, RequestedID, RequestedID);
			
		*Requested = Attack;
		return 1;
	}
unallocated: //goto unallocated if existing allocated array exists
	for (AttackID_t i = 0; i < mgr->Plugins[ID].MaxAttacks; i++) {

		Attack_t **IdxAttack = IndexPluginSpace(mgr, ID, i);
//		write_debug(AddAttackToPlugin, "indexing id=%lu", i);
		assert(IdxAttack != NULL);	// shit's seriously fucked up if this assert fails
		if (*IdxAttack != NULL) continue;
		// found new attack id to use
		
		write_debug(AddAttackToPlugin, "Assigning attack %s to ID %d (req=%d)",
				Attack->AttackName, i, RequestedID);

		*IdxAttack = Attack;

		return 1;
	}

	// couldn't find free attack here.
	return 0;
}

size_t AllocateAttackPlugin(AttackMgr_t *Manager, size_t RequiredPlugins, PluginID_t *ID) {
	if (ID == NULL) return 0;
	size_t RequiredBlocks = ((RequiredPlugins-1) / ATTACK_BLOCK_SIZE)+1;
	size_t FreeBlocksFound = 0;
	int64_t FirstFreeBlock = -1;

	for (size_t i = 0; i < Manager->MaxAttackCount; i++) {
		struct Attacks *Block = &Manager->Attacks[i];
		if (Block->PluginID == (uint32_t)-1) {
			if (FirstFreeBlock == -1)
				FirstFreeBlock = i;
			FreeBlocksFound++;

			if (FreeBlocksFound >= RequiredBlocks) {
				break;
			}
		} else {
			FirstFreeBlock = -1;
			FreeBlocksFound = 0;
		}
	}

	if (FreeBlocksFound < RequiredBlocks) {
		return 0;
	}

	PluginID_t PluginID = GetNewPluginID(Manager,
					FirstFreeBlock,
					RequiredBlocks,
					RequiredPlugins);


	for (size_t i = 0; i < FreeBlocksFound; i++) {
		struct Attacks *Block = &Manager->Attacks[FirstFreeBlock+i];
		Block->PluginID = PluginID;
	}

	*ID = PluginID;
	return FreeBlocksFound;
}

// for plugging into old attack management system
size_t RegisterPluginAttacks(AttackMgr_t *mgr, Registrar_t *Registrar, size_t RegistrarMax) {
	size_t AttacksAdded = 0;

	for (BlockID_t i = 0; i < mgr->BlockIdCount; i++) {
		for (size_t j = 0; j < ATTACK_BLOCK_SIZE; j++) {
			RegistreeID_t ID = (i*ATTACK_BLOCK_SIZE)+j;
			// stop if the max is exceeded
			if (ID >= RegistrarMax) return AttacksAdded;

			Attack_t *Attack = mgr->Attacks[i].Attack[j];

			// no need to do anything if no attack
			if (Attack == NULL) continue;

//			write_debug(RegisterPluginAttacks, "registering attack id = %d", Attack->ID);
			
			Attack_t *RegistrarAttack = (Attack_t*)malloc(sizeof(Attack_t));
			memcpy(RegistrarAttack, Attack, sizeof(Attack_t));

			RegistrarAttack->ID = ID;

			// add the attack
			AttacksAdded += RegistrarAdd(
				Registrar,
				RegistrarAttack,
				RegistrarAttack->ID
			);
		}
	}
}
