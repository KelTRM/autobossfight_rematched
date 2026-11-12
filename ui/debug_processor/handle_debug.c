#include<stdio.h>
#include<stdlib.h>
#include"../../debug/debug.h"
#include"debug_processor.h"

int DetectDebugCommand(const char *IsDbg);

// ProcessDebugCmd
//
// Returns error code
// ERROR CODE RANGE INT_MIN - -1 RESERVED FOR PROCESSOR ERROR
// ERROR CODE 0 = SUCCESS
// ERROR CODE 1 - INT_MAX RESERVED FOR COMMAND ERROR
int ProcessDebugCmd(const char *Cmd) {
	int IsDebug = DetectDebugCommand(Cmd);
	if (IsDebug == 0) return 0;

	Cmd++;

	int argc;
	char *args;
	ssize_t Index = GetCommand(Cmd, &argc, &args);

	if (Index < 0) {
		printf("failed to read command with error %d", Index);
		return -1;
	}

	write_verbose(Info, "Running command @ index %d (%s)",
			Index, CommandList[Index].Name);

	const char **argv = ToTraditionalArgv(argc, args);
	if (argv == NULL) {
		printf("argv allocation error.\n");
		return -2;
	}

	int Result = 0;
	if ((AllowedFlags & CommandList[Index].Flags) == CommandList[Index].Flags) {
		if (CommandList[Index].Handler != NULL) {
			Result = CommandList[Index].Handler(argc, argv);
		} else {
			printf("not implemented.\n");
		}
	} else {
		printf("Command %s found but not available.\n", argv[0]);
	}

	if (Result != 0) {
		printf("command %s failed with code %d\n",
				CommandList[Index].Name, Result);
	}

	free(argv);
	free(args);

	return 1;
}

int DetectDebugCommand(const char *IsDbg) {
	if (IsDbg == NULL) return 0;
	if (*IsDbg == '?') return 1;
	return 0;
}


