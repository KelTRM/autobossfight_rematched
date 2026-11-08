#pragma once

#include<stdlib.h>
#include<stddef.h>
#include"../../entity.h"

int SetHealth(int argc, const char **argv);
int SetForm(int argc, const char **argv);
int GetInfo(int argc, const char **argv);
ssize_t GetCommand(const char *Command, int *argc, char **argv);
const char **ToTraditionalArgv(int argc, const char *argv);
int ParamRange(int argc, const char **argv, int min, int max);
Entity_t *GetEntityFromName(const char *Name);

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

extern const struct Command CommandList[];
