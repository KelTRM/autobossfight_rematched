#include<stdlib.h>
#include<string.h>
#include"transformation.h"
#include"color/color.h"

char *GetTransformationNameStr(Transformation_t *NameStr) {
	if (NameStr->DisplayString != NULL)
		return NameStr->DisplayString;

	Color_t FormColor = GetColorFromHex(NameStr->FormColor);

	char *ColorStr = GetColor(38, FormColor);
	char *ClearStr = GetClearColorStr();

	char *Buffer = malloc(1+strlen(ColorStr) +
				      strlen(ClearStr) +
				      strlen(NameStr->TransformationName));

	// would return TransformationName, but could result in accidentally freeing it
	if (Buffer == NULL) return NULL;

	strcpy(Buffer, ColorStr);
	strcat(Buffer, NameStr->TransformationName);
	strcat(Buffer, ClearStr);

	printf("%d,%d,%d\n",
			FormColor.r,
			FormColor.g,
			FormColor.b
	);

	free(ColorStr);
	
	NameStr->DisplayString = Buffer;

	return Buffer;
}
