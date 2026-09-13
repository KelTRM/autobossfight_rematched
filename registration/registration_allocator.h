#pragma once

#include<stdint.h>
#include"registration.h"

typedef uint32_t	PluginID_t;
typedef int64_t		BlockID_t;

#define BLOCK_SIZE		10					// # of IDs in block
#define MAX_REGISTERS_DEFAULT	65535					// # of IDs (rounded up by block size)

#define MAX_PLUGINS		((MAX_REGISTERS_DEFAULT/BLOCK_SIZE)+1)	// amount of plugins allocated
#define PLUGIN_DEFINED		0xFFFFFFFFul				// Registered value for valid plugins
#define INVALID_PLUGIN_ID	0xFFFFFFFFul				// ID for invalid plugin

// plugin id refers to index into array
struct PluginRegistry {
	uint32_t Registered;		// 0xFFFFFFFF if registered, anything else otherwise
	
	BlockID_t FirstRegisteredBlock;
	size_t AllocatedBlockCount;

	size_t MaxRegistrations;
};

typedef struct Registration {
	void *Registration;
	RegistreeID_t ID;
} Registration_t;

typedef struct Registrations {
	struct Block {
		Registration_t Registration[BLOCK_SIZE];
		uint32_t PluginID;
	} *Blocks;

	BlockID_t BlockIdCount;
	size_t MaxRegistrationCount;
	struct PluginRegistry Plugins[MAX_PLUGINS];
} RegistrationMgr_t;

/* OpenPluginAllocator(MaxRegistrations)
 * MaxRegistrations: Maximum amount of registrations. 0 to use default
 *
 * Returns a new registration manager
 * Note - Max registrations is rounded up to the nearest block
 */
RegistrationMgr_t OpenPluginAllocator(size_t MaxRegistrations);

/* GetPluginSizeFromID(mgr, id)
 * RegistrationMgr_t: The manager which the plugin is registered to
 * PluginID_t ID: The ID of the plugin
 *
 * Returns the size of the plugin
 */
size_t GetPluginSizeFromID(RegistrationMgr_t *mgr, PluginID_t ID);
int ValidatePlugin(RegistrationMgr_t *mgr, PluginID_t ID);
Registration_t *IndexPluginSpace(RegistrationMgr_t *mgr, PluginID_t ID, RegistreeID_t RegistrationID);
size_t AddRegistrationToPlugin(RegistrationMgr_t *mgr, PluginID_t ID, void *Registration);
size_t AllocatePlugin(RegistrationMgr_t *Manager, size_t RequiredPlugins, PluginID_t *ID);
size_t RegisterPlugins(RegistrationMgr_t *mgr, Registrar_t *Registrar, size_t RegistrarMax);
