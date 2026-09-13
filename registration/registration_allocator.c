#include<assert.h>
#include<stdlib.h>
#include<string.h>
#include"registration_allocator.h"
#include"registration.h"
#include"../debug/debug.h"

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
RegistrationMgr_t OpenPluginAllocator(size_t MaxAttacks) {
	if (MaxAttacks == 0)
		MaxAttacks = MAX_REGISTERS_DEFAULT;

	RegistrationMgr_t Manager;
	Manager.MaxRegistrationCount = ((MaxAttacks-1) / BLOCK_SIZE)+1;
	Manager.Blocks = calloc(Manager.MaxRegistrationCount, sizeof(struct Block));

	Manager.BlockIdCount = Manager.MaxRegistrationCount / BLOCK_SIZE;

	if (Manager.Blocks == NULL)
		return (RegistrationMgr_t){ 0 };

	for (size_t i = 0; i < Manager.MaxRegistrationCount; i++) {
		memset(Manager.Blocks[i].Registration, 0, sizeof(Registration_t) * BLOCK_SIZE);
		Manager.Blocks[i].PluginID = -1;
	}

	return Manager;
}

size_t GetPluginSizeFromID(RegistrationMgr_t *mgr, PluginID_t ID) {
	if (mgr->Plugins[ID].Registered != PLUGIN_DEFINED) return 0;
	return mgr->Plugins[ID].MaxRegistrations;
//	return 0;
}

static PluginID_t GetNewPluginID(RegistrationMgr_t *mgr,
				BlockID_t FirstBlock,
				size_t BlockCount,
				size_t RegistrationCount) {
	for (PluginID_t i = 0; i < MAX_PLUGINS; i++) {
		if (mgr->Plugins[i].Registered == PLUGIN_DEFINED) continue;

		// PluginID = i
		mgr->Plugins[i].Registered = PLUGIN_DEFINED;
		mgr->Plugins[i].MaxRegistrations = RegistrationCount;
		mgr->Plugins[i].FirstRegisteredBlock = FirstBlock;
		mgr->Plugins[i].AllocatedBlockCount = BlockCount;
		return i;
	}
	return INVALID_PLUGIN_ID;
}

int ValidatePlugin(RegistrationMgr_t *mgr, PluginID_t ID) {
	if (ID >= MAX_PLUGINS) return 0;
	if (mgr == NULL) return 0;
	if (mgr->Plugins[ID].Registered != PLUGIN_DEFINED) return 0;

	return 1;
}

static Registration_t *IndexAttackManager(RegistrationMgr_t *mgr, RegistreeID_t ID) {
	if (ID > mgr->MaxRegistrationCount)
		return NULL;

	BlockID_t Block = ID / BLOCK_SIZE;
	RegistreeID_t BlockIdx = ID % BLOCK_SIZE;

	return &mgr->Blocks[Block].Registration[BlockIdx];
}

Registration_t *IndexPluginSpace(RegistrationMgr_t *mgr, PluginID_t ID, RegistreeID_t Registration) {
	if (ID == INVALID_PLUGIN_ID) IndexAttackManager(mgr, Registration);
	if (ValidatePlugin(mgr, ID) == 0) return NULL;

	size_t MaxRegistrations = mgr->Plugins[ID].MaxRegistrations;
	if (Registration > MaxRegistrations) return NULL;

	BlockID_t PluginBlock = mgr->Plugins[ID].FirstRegisteredBlock;
	RegistreeID_t GlobalStartingID = PluginBlock * BLOCK_SIZE;

	return IndexAttackManager(mgr, GlobalStartingID + Registration);
}

size_t AddAttackToPlugin(RegistrationMgr_t *mgr, PluginID_t ID, RegistreeID_t RequestedID, void *Registration) {
	// make sure the plugin exists
	if (ValidatePlugin(mgr, ID) == 0) return 0;

	// confirm attack exists
	if (Registration == NULL)
		return 0;

//	RegistreeID_t RequestedID = Attack->ID;

	if (RequestedID != 0) {
		Registration_t *Requested = IndexPluginSpace(mgr, ID, RequestedID);
		if (Requested != NULL) {
//			write_debug(AddAttackToPlugin, "ExistingAttack->ID = %d",
//					ExistingAttack->ID);
			if (Requested->ID == RequestedID) {
				// requested ID taken. go after different spot
				goto unallocated;
			}

			void *Registration = Requested->Registration;
			Requested->Registration = NULL;

			// a little recursion never hurts
			AddAttackToPlugin(mgr, ID, RequestedID, Registration);
			AddAttackToPlugin(mgr, ID, 0, Registration);
		}

//		write_debug(AddAttackToPlugin, "Assigning attack %s to ID %d (req=%d)",
//				Attack->AttackName, RequestedID, RequestedID);
			
		Requested->Registration = Registration;
		return 1;
	}
unallocated: //goto unallocated if existing allocated array exists
	for (RegistreeID_t i = 0; i < mgr->Plugins[ID].MaxRegistrations; i++) {

		Registration_t *IdxAttack = IndexPluginSpace(mgr, ID, i);
		// found new id to use

		IdxAttack->Registration = Registration;

		return 1;
	}

	// couldn't find free attack here.
	return 0;
}

size_t AllocateAttackPlugin(RegistrationMgr_t *Manager, size_t RequiredPlugins, PluginID_t *ID) {
	if (ID == NULL) return 0;
	size_t RequiredBlocks = ((RequiredPlugins-1) / BLOCK_SIZE)+1;
	size_t FreeBlocksFound = 0;
	int64_t FirstFreeBlock = -1;

	for (size_t i = 0; i < Manager->MaxRegistrationCount; i++) {
		struct Block *Block = &Manager->Blocks[i];
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
		struct Block *Block = &Manager->Blocks[FirstFreeBlock+i];
		Block->PluginID = PluginID;
	}

	*ID = PluginID;
	return FreeBlocksFound;
}

// for plugging into old attack management system
size_t RegisterPlugins(RegistrationMgr_t *mgr, Registrar_t *Registrar, size_t RegistrarMax) {
	size_t RegistrationsAdded = 0;

	for (RegistreeID_t ID = 0; ID < mgr->MaxRegistrationCount; ID++) {
		if (ID >= RegistrarMax) break;

		Registration_t *Registration = IndexAttackManager(mgr, ID);
		RegistrarAdd(
			Registrar,
			Registration->Registration,
			Registration->ID
		);
	}
//	for (BlockID_t i = 0; i < mgr->BlockIdCount; i++) {
//		for (size_t j = 0; j < BLOCK_SIZE; j++) {
//			RegistreeID_t ID = (i*BLOCK_SIZE)+j;
//			// stop if the max is exceeded
//			if (ID >= RegistrarMax) return RegistrationsAdded;
//
//			Registration_t *Registrations = mgr->Blocks[i].Registration[j];
//
//			// no need to do anything if no attack
//			if (Attack == NULL) continue;
//
//			write_debug(RegisterPluginAttacks, "registering attack id = %d", Attack->ID);
//			
//			Attack_t *RegistrarAttack = (Attack_t*)malloc(sizeof(Attack_t));
//			memcpy(RegistrarAttack, Attack, sizeof(Attack_t));
//
//			RegistrarAttack->ID = ID;
//
//			// add the attack
//			AttacksAdded += RegistrarAdd(
//				Registrar,
//				RegistrarAttack,
//				RegistrarAttack->ID
//			);
//		}
//	}

	return RegistrationsAdded;
}
