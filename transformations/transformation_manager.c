#include"transformation_manager.h"
#include "linked_list/list.h"
#include "transformation.h"
#include<registration.h>

#define MAX_TRANSFORMATIONS	256

Registrar_t Registrar;

int InitTransformationRegistrar(void) {
	return InitRegistrar(MAX_TRANSFORMATIONS, &Registrar);
}

int RegisterTransformation(Transformation_t *Transformation) {
	return RegistrarAdd(&Registrar, Transformation, 0);
}

Transformation_t *GetTransformationAtIndex(size_t Index) {
	return (Transformation_t*)GetRegistrationFromID(&Registrar, Index);
}

size_t BuildTransformationList(void) {
	return BuildRegistrationList(&Registrar);
}

TransformationIter_t OpenTransformationIterator(void) {
	return OpenRegistrationIterator(&Registrar);
}

Transformation_t *StepTransformationIterator(TransformationIter_t *Iterator) {
	return StepRegistrationIterator((Linked_t*)Iterator);
}
