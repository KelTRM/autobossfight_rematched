#include <stdio.h>
#include<stdlib.h>
#include"../transformations/transformation.h"
#include"../ui/strings.h"

char *GetEntityNameFormDisp(Entity_t *Entity, size_t NamePadding, size_t FormPadding) {
	char *NameColoredStr = GetEntityNameStr(Entity);
	
	size_t ColoredLen = strlen(NameColoredStr);
	size_t NormalLen = strlen(Entity->Name);

	size_t ColoredAdditionalChars = ColoredLen - NormalLen;

	char *NameStr = PadRight(NameColoredStr, NamePadding + ColoredAdditionalChars, ' ');
	free(NameColoredStr);
	if (NameStr == NULL) return NULL;

	const char *FormString = GetTransformationNameStr(Entity->EntityTransformation);
	const char *RegularStr = Entity->EntityTransformation->TransformationName;
	
	if (FormString == NULL) FormString = RegularStr;
	if (FormString == NULL) {
		free(NameStr);
		return NULL;
	}

	size_t PaddingAmount = (strlen(FormString) - strlen(RegularStr))+FormPadding+2;

	char *str = alloca(PaddingAmount+1);
	snprintf(str, PaddingAmount+1, "[%s]", FormString);

	char *FormStr = PadLeft(str, PaddingAmount, ' ');

	size_t len = snprintf(NULL, 0, "%s %s", NameStr, FormStr);
	char *Buffer = malloc(len+1);

	if (Buffer == NULL) {
		free(NameStr);
		free(FormStr);
		return NULL;
	}

	snprintf(Buffer, len+1, "%s %s", NameStr, FormStr);

	free(NameStr);
	free(FormStr);

	return Buffer;
}
