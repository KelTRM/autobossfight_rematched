#pragma once

#include"transformation.h"
#include<stddef.h>

typedef void *TransformationIter_t;

int InitTransformationRegistrar(void);
int RegisterTransformation(Transformation_t *Transformation);
Transformation_t *GetTransformationAtIndex(size_t Index);

size_t BuildTransformationList(void);
TransformationIter_t OpenTransformationIterator(void);
Transformation_t *StepTransformationIterator(TransformationIter_t *Iterator);
