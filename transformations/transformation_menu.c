#include"transformation_manager.h"
#include"../utils/sleep.h"
#include<stdlib.h>
#include<ui.h>

Transformation_t *AskTransformation(void) {
	BUFHANDLE MainBuffer = ActiveBuffer;
	BUFHANDLE Buffer = CreateBuffer();

	TransformationIter_t Iterator = OpenTransformationIterator();
	Transformation_t *CurrentTransformation;

	while (Iterator != NULL) {
		CurrentTransformation = StepTransformationIterator(&Iterator);
		bprintf(Buffer, "To use form [%s] use %lu -- %s (%lu cost. %lu%% upkeep)\n",
			CurrentTransformation->Name,
			CurrentTransformation->ID,
			CurrentTransformation->FormDescription,
			CurrentTransformation->Cost,
			CurrentTransformation->Upkeep
		);
	}

	SwitchBuffer(Buffer);

	while (1) {
		char *Response = NULL;
		Prompt("\nSelect a transformation.", &Response, 0);

		if (Response == NULL) {
			printf("Failed to read a response. Please try again.\n");
			sleep(1000);
			continue;
		}

		TransformationID_t Choice;
		Choice = atoll(Response);

		CurrentTransformation = GetTransformationAtIndex(Choice);
		if (CurrentTransformation == NULL || Choice == 0) {
			printf("Unknown transformation '%s'. Please select a valid transformation\n",
					Response);
			free(Response);

			sleep(1000);
			continue;
		}

		break;
	}

	SwitchBuffer(MainBuffer);

	return CurrentTransformation;
}
