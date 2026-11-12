#include<stdlib.h>
#include<string.h>
#include"debug_processor.h"

const struct Command CommandList[] = {
	{ .Name="sethealth",.Handler=SetHealth,	.Flags=CMD_DEBUG },
	{ .Name="transform",.Handler=SetForm,	.Flags=CMD_DEBUG },
	{ .Name="info",	.Handler=GetInfo,	.Flags=CMD_INFO }
};

// ssize_t GetCommand(const char *Command, int *argc, const char **argc);
// Returns an index of CommandList containing the current command,
// along with the argc/argv for the command
//
// int *argc: Contains the number of args to the command
// const char **argv: A null-separated list of arguments. Needs to be freed after use
ssize_t GetCommand(const char *Command, int *argc, char **argv) {
	// start by parsing argv
	size_t CommandLength = strlen(Command);

	*argv = malloc(CommandLength+1);
	if (*argv == NULL) return -1;

	memcpy(*argv, Command, CommandLength+1);

	size_t BytesAtLast = 0;

	*argc = 0;
	size_t i;
	for (i = 0; i < CommandLength; i++) {
		char *ch = (*argv) + i;
		if (*ch == ' ') {
			*ch = 0;
			(*argc)++;
		}
	}
	if (BytesAtLast != i)
		(*argc)++;

	// *argv = command name
	for (size_t i = 0; i < sizeof(CommandList)/sizeof(struct Command); i++) {
		const char *CommandName = CommandList[i].Name;
		if (strcmp(CommandName, *argv) == 0) {
			return i;
		}
	}

	return -1;
}

// char **ToTraditionalArgv(int, const char*);
//
// Takes in null-separated argv and converts it
// to the traditional double-pointer argv
//
// Must be freed afterwards.
const char **ToTraditionalArgv(int argc, const char *argv) {
	if (argv == NULL) return NULL;

	const char **ptr = calloc(argc, sizeof(const char*));
	if (ptr == NULL) return NULL;

	for (int i = 0; i < argc; i++) {
		ptr[i] = argv;
		while (*(argv++) != '\0') { }
	}

	return ptr;
}

