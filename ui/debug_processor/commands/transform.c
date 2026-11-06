#include<string.h>
#include<limits.h>
#include"../transformations/transformation_manager.h"
#include"../debug_processor.h"

int SetForm(int argc, const char **argv) {
	if (ParamRange(argc, argv, 3, INT_MAX))
		return 1;

	Entity_t *Target = GetEntityFromName(argv[1]);
	if (Target == NULL) {
		printf("Unable to find entity %s\n", argv[1]);
		return 2;
	}

	TransformationIter_t Iter = OpenTransformationIterator();
	Transformation_t *Form;
	while (1) {
		Form = StepTransformationIterator(&Iter);
		if (Form == NULL) break;

		const char *Name = Form->Identifier;
		if (strcmp(Name, argv[2]) == 0) break;
	}

	if (Form == NULL) {
		printf("Unable to find form %s\n", argv[2]);
		return 3;
	}

	Target->EntityTransformation = Form;

	printf("Set form of entity %s to %s\n",
			Target->Name, Form->TransformationName);

	return 0;
}

