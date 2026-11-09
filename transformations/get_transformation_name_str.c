#include<stdlib.h>
#include<string.h>
#include"transformation.h"

char *GetTransformationNameStr(Transformation_t *NameStr) {
	if (NameStr->DisplayString != NULL)
		return NameStr->DisplayString;

	char *ColorStr = GetColor(38, NameStr->FormColor);
	char *ClearStr = GetClearColorStr();

	char *Buffer = malloc(1+strlen(ColorStr) +
				      strlen(ClearStr) +
				      strlen(NameStr->TransformationName));
	strcpy(Buffer, ColorStr);
	strcat(Buffer, NameStr->TransformationName);
	strcat(Buffer, ClearStr);

	printf("%d,%d,%d\n",
			NameStr->FormColor.r,
			NameStr->FormColor.g,
			NameStr->FormColor.b
	);

	free(ColorStr);
	
	NameStr->DisplayString = Buffer;

	return Buffer;
}
