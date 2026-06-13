#include<stdio.h>
#include<stdlib.h>
#include"ui.h"

// Prompts user for input.
// Result - String of user input of arbitrary length. Must be freed
// EchoToBuffer - Echos result to buffer if true.
void Prompt(const char *Prompt, char **Result, int EchoToBuffer) {
	size_t Length = 0;
	char *CurrentBuffer = malloc(Length+1);

	if (CurrentBuffer == NULL)
		goto nullbuffer;

	//fflush(stdin);
	
	printf("%s ", Prompt);

	CurrentBuffer[0] = 0;

	char ch;
	do {
		ch = getchar();
		if (ch == '\r' || ch == '\n') continue;

		Length++;
		char *NewBuffer = realloc(CurrentBuffer, Length+1);

		if (NewBuffer == NULL)
			goto nullbuffer;

		NewBuffer[Length-1] = ch;
		NewBuffer[Length] = 0;

		CurrentBuffer = NewBuffer;
	} while (ch != '\n');


	if (EchoToBuffer != 0) {
		RefreshScreen();
		printf(CurrentBuffer);
	}

	*Result = CurrentBuffer;

	return;

nullbuffer:
//	printf("Recieved null on Prompt.");
	if (CurrentBuffer != NULL)
		free(CurrentBuffer);
	*Result = NULL;
}
