#define USE_STD_STRLEN

#include<stdlib.h>
#include<string.h>
#include"../debug/debug.h"
#include"console_manager/console.h"
#include"ui.h"

struct Command {
	const char *Name;
	int (*Handler)(const char *Parameters);
};

static int SetHealth(const char *Parameters) {
	return (int)*Parameters;
}

static const struct Command CommandList[] = {
	{ .Name="sethealth", .Handler=SetHealth }
};

int DetectDebugCommand(const char *IsDbg) {
	if (IsDbg == NULL) return 0;
	if (*IsDbg == '?') return 1;
	return 0;
}

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

int ProcessDebugCmd(const char *Cmd) {
	int IsDebug = DetectDebugCommand(Cmd);
	if (IsDebug == 0) return 0;

	Cmd++;

	int argc;
	char *argv;
	ssize_t Index = GetCommand(Cmd, &argc, &argv);

	write_verbose(Info, "Running command @ index %d (%s)",
			Index, CommandList[Index].Name);

	return 1;
}
