#include"transformation_manager.h"
#include<ui.h>

Transformation_t *AskTransformation(void) {
	BUFHANDLE MainBuffer = ActiveBuffer;
	BUFHANDLE Buffer = CreateBuffer();

	TransformationIter_t Iterator = OpenTransformationIterator();
	Transformation_t *CurrentTransformation;

	while (Iterator != NULL) {
		CurrentTransformation = StepTransformationIterator(&Iterator);
		bprintf(Buffer, "[%s] (%lu cost. %lu%% upkeep)\n",
			CurrentTransformation->Name,
			CurrentTransformation->Cost,
			CurrentTransformation->Upkeep
		);
	}

	SwitchBuffer(Buffer);

	char *Response;
	Prompt("Select a transformation.", &Response, 0);

	SwitchBuffer(MainBuffer);
}
