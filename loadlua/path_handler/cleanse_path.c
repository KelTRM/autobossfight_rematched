#include<stdio.h>
#include<stdlib.h>
#include<string.h>

enum States {
	STATE_NEXT_SEGMENT,
	STATE_READ_FILENAME,
	STATE_CURRENT_DIR,
	STATE_PARENT_DIR,
	STATE_ERR
};

char *CleanseFilePath(const char *Filename) {
	if (Filename == NULL)
		return NULL;

	if (*Filename == 0)
		return NULL;

	char **Locations = calloc(sizeof(char*), 1);
	if (Locations == NULL)
		return NULL;

	enum States CurrentState = STATE_NEXT_SEGMENT;
	*Locations = NULL;

	size_t CurrentIndex = 0;

	while (*Filename != 0) {
		switch (CurrentState) {
			case STATE_NEXT_SEGMENT: {
				if (Locations[CurrentIndex] != NULL)
					CurrentIndex++;

				char **ptr = realloc(Locations, sizeof(char*) * CurrentIndex+1);
				if (ptr == NULL) {
					CurrentState = STATE_ERR;
					continue;
				}

				Locations = ptr;
				
				Locations[CurrentIndex] = malloc(1);
				*Locations[CurrentIndex] = 0;

				CurrentState = STATE_READ_FILENAME;
			} break;
			case STATE_READ_FILENAME: {
				char *CurrentPtr = Locations[CurrentIndex];
				if (CurrentPtr == NULL) {
					CurrentState = STATE_ERR;
					continue;
				}

				char ch = *Filename;
				
				if (ch == '/') {
					CurrentState = STATE_NEXT_SEGMENT;
				}

				size_t Bytes = strlen(CurrentPtr);
				CurrentPtr = realloc(CurrentPtr, Bytes+1);

				if (CurrentPtr == NULL) {
					CurrentState = STATE_ERR;
					continue;
				}

				Filename++;
				
				CurrentPtr[Bytes] = ch;
				CurrentPtr[Bytes+1] = 0;

				Locations[CurrentIndex] = CurrentPtr;
			} break;
			case STATE_CURRENT_DIR: {
				
			} break;

			case STATE_ERR: {
				for (size_t i = 0; i < CurrentIndex; i++) {
					if (Locations[CurrentIndex] != NULL)
						free(Locations[CurrentIndex]);
				}

				free(Locations);

				return NULL;
			} break;

			default:	printf("Unknown state.\n");
		}

		Filename++;
	}

	printf("CurrentIndex = %zu\n", CurrentIndex);
	for (size_t i = 0; i < CurrentIndex; i++) {
		printf("Locations[%zu]=%s\n", i, Locations[CurrentIndex]);
	}

	return "";
}

char **SplitPath(const char *Path) {
	char **Step = malloc(sizeof(char**));
	if (Step == NULL)
		return NULL;

	char **CurrentStep = &Step[0];


}
