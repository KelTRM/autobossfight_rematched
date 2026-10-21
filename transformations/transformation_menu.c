#include<stdio.h>
#include<stdlib.h>
#include<alloca.h>
#include<ui.h>
#include<strings.h>
#include"transformation_manager.h"
#include"../utils/sleep.h"
#include"../debug/debug.h"

Transformation_t *AskTransformation(void) {
	BUFHANDLE MainBuffer = ActiveBuffer;
	BUFHANDLE Buffer = CreateBuffer();

	TransformationIter_t Iterator = OpenTransformationIterator();
	Transformation_t *CurrentTransformation;

	size_t MaxFormLength = 0;
	while (Iterator != NULL) {
		CurrentTransformation = StepTransformationIterator(&Iterator);
		write_verbose(Debug, "Iterating over %s", CurrentTransformation->TransformationName);
		MaxFormLength = max(
			MaxFormLength,
			strlen(CurrentTransformation->TransformationName)
		);
	}

	Iterator = OpenTransformationIterator();

	while (Iterator != NULL) {
		CurrentTransformation = StepTransformationIterator(&Iterator);
		write_verbose(Debug, "got ptr=%p", CurrentTransformation);

		char *FormBuffer = alloca(MaxFormLength+3);
		snprintf(FormBuffer, MaxFormLength+3,
				"[%s]", CurrentTransformation->TransformationName);

		char *FormDisp = PadRight(FormBuffer, MaxFormLength+2, ' ');
		bprintf(Buffer, "To use form %s use %lu -- %s (%lu cost. %lu%% upkeep)\n",
			FormDisp,
//			CurrentTransformation->TransformationName,
			CurrentTransformation->ID,
//			1,
//			CurrentTransformation->FormDescription,
			NULL,
			0,
			0
//			CurrentTransformation->Cost,
//			CurrentTransformation->Upkeep
		);

		free(FormDisp);
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
