#include<stddef.h>
#include"transformation_manager.h"

const Transformation_t *TransformationsToRegister[] = {
	&BaseTransformation,
	&TestTransformation
};

size_t InitTransformations(void) {
	InitTransformationRegistrar();

	size_t TransformationCount =
			sizeof(TransformationsToRegister) /
			sizeof(*TransformationsToRegister);
	size_t RegisteredAttacks = 0;

	for (size_t i = 0; i < TransformationCount; i++) {
		RegisteredAttacks +=
			RegisterTransformation(
				(Transformation_t*)TransformationsToRegister[i]
			);
	}

	BuildTransformationList();

	return RegisteredAttacks;
}
