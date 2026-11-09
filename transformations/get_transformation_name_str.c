#include<stdlib.h>
#include<string.h>
#include"transformation.h"

char *GetTransformationNameStr(Transformation_t *NameStr) {
	printf("%s, %p\n", NameStr->TransformationName,
			NameStr->DisplayString);
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
	
	// causing a segfault for some reason? debugger says it should be fine but it segfaults anyway
	// NameStr->DisplayString is a valid, allocated entry to NameStr, Buffer is fine.
	// DisplayString reads as a NULL pointer
	// fuck this shit
	NameStr->DisplayString = (char*)0x0001;

	return Buffer;
}
