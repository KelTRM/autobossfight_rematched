#define USE_STD_STRLEN

#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include"../debug/debug.h"
#include"../transformations/transformation_manager.h"
#include"console_manager/console.h"
#include"ui.h"

static int SetHealth(int argc, const char **argv);
static int SetForm(int argc, const char **argv);

extern Entity_t *Entities;
extern size_t EntityCount;

enum CommandType {
	CMD_INFO=0x00,
	CMD_DEBUG=0x01,
};

// Specifies what flags are allowed to be on.
// Any command where a disallowed flag is enabled will fail
static const int AllowedFlags = 0x0001;

struct Command {
	const char *Name;
	int (*Handler)(int argc, const char **argv);
	int Flags;
};

static const struct Command CommandList[] = {
	{ .Name="sethealth",.Handler=SetHealth,	.Flags=CMD_DEBUG },
	{ .Name="transform",.Handler=SetForm,	.Flags=CMD_DEBUG },
	{ .Name="info",	.Handler=NULL,		.Flags=CMD_INFO }
};

Entity_t *GetEntityFromName(const char *Name) {
	Entity_t *Entity = NULL;
	for (size_t i = 0; i < EntityCount; i++) {
		int r = strcmp(Entities[i].Name, Name);
		if (r != 0) continue;

		Entity = Entities + i;
		break;
	}

	return Entity;
}

int ParamRange(int argc, const char **argv, int min, int max) {
	if (argc < min) {
		printf("Expected at least %d parameters for command %s. Got %d",
				min, argv[0], argc);
		return 1;
	}

	if (argc > max) {
		printf("Expected at most %d parameters for command %s. Got %d",
				max, argv[0], argc);
		return 1;
	}

	return 0;
}

static int SetHealth(int argc, const char **argv) {
	if (ParamRange(argc, argv, 3, INT_MAX))
		return 1;

	// find entity from parameter 1
	const char *Entity = argv[1];
	Entity_t *Target = GetEntityFromName(Entity);

	if (Target == NULL) {
		printf("Unable to find entity %s.\n", Entity);
		return 2;
	}

	Health_t NewHealth = atoll(argv[2]);
	Target->HealthPoints = NewHealth;

	printf("%s's health set to %d.\n", Entity, NewHealth);

	return 0;
}

static int SetForm(int argc, const char **argv) {
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

// char **ToTraditionalArgv(int, const char*);
//
// Takes in null-separated argv and converts it
// to the traditional double-pointer argv
//
// Must be freed afterwards.
const char **ToTraditionalArgv(int argc, const char *argv) {
	const char **ptr = calloc(argc, sizeof(const char*));

	for (int i = 0; i < argc; i++) {
		ptr[i] = argv;
		while (*(argv++) != '\0') { }
	}

	return ptr;
}

int ProcessDebugCmd(const char *Cmd) {
	int IsDebug = DetectDebugCommand(Cmd);
	if (IsDebug == 0) return 0;

	Cmd++;

	int argc;
	char *args;
	ssize_t Index = GetCommand(Cmd, &argc, &args);

	write_verbose(Info, "Running command @ index %d (%s)",
			Index, CommandList[Index].Name);

	const char **argv = ToTraditionalArgv(argc, args);

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
