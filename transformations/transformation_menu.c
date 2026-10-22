#include<stdio.h>
#include<stdlib.h>
#include<alloca.h>
#include<ui.h>
#include"strings.h"
#include"transformation_manager.h"
#include"../utils/sleep.h"
#include"../debug/debug.h"

Transformation_t *AskTransformation(void) {
	BUFHANDLE MainBuffer = ActiveBuffer;
	BUFHANDLE Buffer = CreateBuffer();

	TransformationIter_t Iterator = OpenTransformationIterator();
	Transformation_t *CurrentTransformation;

	size_t MaxFormLength = 0;

	TransformationID_t MaxID = 0;
	while (Iterator != NULL) {
		CurrentTransformation = StepTransformationIterator(&Iterator);
		MaxFormLength = max(
			MaxFormLength,
			strlen(CurrentTransformation->TransformationName)
		);

		MaxID = max(MaxID, CurrentTransformation->ID);
	}

	Iterator = OpenTransformationIterator();
	MaxID = CalculateDigitsInNumber(MaxID);

	while (Iterator != NULL) {
		CurrentTransformation = StepTransformationIterator(&Iterator);

		char *FormBuffer = alloca(MaxFormLength+3);
		snprintf(FormBuffer, MaxFormLength+3,
				"[%s]", CurrentTransformation->TransformationName);

		char *FormDisp = PadRight(FormBuffer, MaxFormLength+2, ' ');

		char *IdBuffer = alloca(MaxID+1);
		snprintf(IdBuffer, MaxID+3, "%lu",
				CurrentTransformation->ID);

		char *IdStr = PadLeft(IdBuffer, MaxID, ' ');

		bprintf(Buffer, "To use form %s use %s -- %s (%lu cost. %lu%% upkeep)\n",
			FormDisp, IdStr,
			CurrentTransformation->FormDescription,
			CurrentTransformation->Cost,
			CurrentTransformation->Upkeep
		);

		free(FormDisp);
		free(IdStr);
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
