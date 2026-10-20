#include<stddef.h>
#include"registration.h"
#include"registration_allocator.h"
#include"strings.h"
#include"transformation.h"
#include"transformation_manager.h"
#include"../debug/debug.h"

const Transformation_t *TransformationsToRegister[] = {
	&BaseTransformation,
	&SuperTransformation
};

const Transformation_t *DefaultTransformation = &BaseTransformation;
extern Registrar_t Registrar;

size_t InitTransformations(void) {
	InitTransformationRegistrar();

	RegistrationMgr_t mgr = OpenPluginAllocator(0);

	size_t TransformationCount =
			sizeof(TransformationsToRegister) /
			sizeof(*TransformationsToRegister);
	size_t RegisteredAttacks = 0;

	size_t MaxID = 0;

	for (size_t i = 0; i < TransformationCount; i++) {
		MaxID = max(MaxID, TransformationsToRegister[i]->ID);
	}

	PluginID_t PluginID;
	size_t Size = AllocatePlugin(&mgr,
			max(MaxID, TransformationCount),
			&PluginID);

	write_verbose(Info, "Loaded vanilla transformation plugin of block count=%zu", Size);

	for (size_t i = 0; i < TransformationCount; i++) {
		RegisteredAttacks +=
			AddRegistrationToPlugin(&mgr, PluginID,
				(void*)TransformationsToRegister[i],
				(void*)offsetof(Transformation_t, ID),
				sizeof(Transformation_t)
			);
//			RegisterTransformation(
//				(Transformation_t*)TransformationsToRegister[i]
//			);
	}

	RegisterPlugins(&mgr, &Registrar, 256);

	BuildTransformationList();

	return RegisteredAttacks;
}
