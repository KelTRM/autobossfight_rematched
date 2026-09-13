#include<assert.h>
#include<stdlib.h>
#include<string.h>
#include"registration_allocator.h"
#include"registration.h"
#include"../debug/debug.h"

typedef uint32_t	PluginID_t;
typedef int64_t		BlockID_t;

/*
 * --- PLUGIN ID MODEL ---
 *
 *  Registration IDs will be allocated within the maximum registration count.
 *  the IDs will shall be allocated in a first-come first-serve manner by plugins
 *  The allocated section of IDs will act as a virtual address space within the plugin.
 *
 *  The plugins which specifically request an ID get priority, while those which request no specific ID
 *  will get lower priority. (If they're registered in order of { 0, 1, 0 }, they will get IDs of { 2, 1, 3 })
 */

// MaxRegistrations = 0 gives default
RegistrationMgr_t OpenPluginAllocator(size_t MaxRegistrations) {
	if (MaxRegistrations == 0)
		MaxRegistrations = MAX_REGISTERS_DEFAULT;

	RegistrationMgr_t Manager;
	Manager.MaxRegistrationCount = ((MaxRegistrations-1) / BLOCK_SIZE)+1;
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

static Registration_t *IndexManager(RegistrationMgr_t *mgr, RegistreeID_t ID) {
	if (ID > mgr->MaxRegistrationCount)
		return NULL;

	BlockID_t Block = ID / BLOCK_SIZE;
	RegistreeID_t BlockIdx = ID % BLOCK_SIZE;

	write_debug(Debug, "Indexing into registration manager @ (%d,%d)", Block, BlockIdx);

	return &mgr->Blocks[Block].Registration[BlockIdx];
}

Registration_t *IndexPluginSpace(RegistrationMgr_t *mgr, PluginID_t ID, RegistreeID_t Registration) {
	if (ID == INVALID_PLUGIN_ID) IndexManager(mgr, Registration);
	if (ValidatePlugin(mgr, ID) == 0) return NULL;

	size_t MaxRegistrations = mgr->Plugins[ID].MaxRegistrations;
	if (Registration > MaxRegistrations) return NULL;

	BlockID_t PluginBlock = mgr->Plugins[ID].FirstRegisteredBlock;
	RegistreeID_t GlobalStartingID = PluginBlock * BLOCK_SIZE;

	return IndexManager(mgr, GlobalStartingID + Registration);
}

size_t AddRegistrationToPlugin(RegistrationMgr_t *mgr, PluginID_t ID, RegistreeID_t RequestedID, void *Registration) {
	// make sure the plugin exists
	if (ValidatePlugin(mgr, ID) == 0) return 0;

	// confirm registration exists
	if (Registration == NULL)
		return 0;

	if (RequestedID != 0) {
		Registration_t *Requested = IndexPluginSpace(mgr, ID, RequestedID);
		if (Requested->Registration != NULL) {
			if (Requested->ID == RequestedID) {
				// requested ID taken. go after different spot
				goto unallocated;
			}

			void *Registration = Requested->Registration;
			Requested->Registration = NULL;

			// a little recursion never hurts
			AddRegistrationToPlugin(mgr, ID, RequestedID, Registration);
			AddRegistrationToPlugin(mgr, ID, 0, Registration);
		}
			
		Requested->Registration = Registration;
		Requested->ID = RequestedID;
		return 1;
	}
unallocated: //goto unallocated if existing allocated array exists
	for (RegistreeID_t i = 0; i < mgr->Plugins[ID].MaxRegistrations; i++) {

		Registration_t *Registration = IndexPluginSpace(mgr, ID, i);
		// found new id to use

		Registration->Registration = Registration;
		Registration->ID = RequestedID;

		return 1;
	}

	// couldn't find free ID here.
	return 0;
}

size_t AllocatePlugin(RegistrationMgr_t *Manager, size_t RequiredPlugins, PluginID_t *ID) {
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

// for plugging into old registration management system
size_t RegisterPlugins(RegistrationMgr_t *mgr, Registrar_t *Registrar, size_t RegistrarMax) {
	size_t RegistrationsAdded = 0;

	for (RegistreeID_t ID = 0; ID < mgr->MaxRegistrationCount; ID++) {
		if (ID >= RegistrarMax) break;

		Registration_t *Registration = IndexManager(mgr, ID);
		RegistrarAdd(
			Registrar,
			Registration->Registration,
			Registration->ID
		);
	}

	return RegistrationsAdded;
}
