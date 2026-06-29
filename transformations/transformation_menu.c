#include"transformation_manager.h"
#include<ui.h>

Transformation_t *AskTransformation() {
	TransformationIter_t Iterator = OpenTransformationIterator();
	Transformation_t *CurrentTransformation;

	while ((CurrentTransformation=StepTransformationIterator(&Iterator)) != NULL) {
		printf("[%s] (%lu%%/round. %lu upfront\n",
				CurrentTransformation->Name,
				CurrentTransformation->Upkeep,
				CurrentTransformation->Cost);
	}
}
