#include"ui.h"
#include"console_manager/console.h"

int DetectDebugCommand(const char *IsDbg) {
	if (IsDbg == NULL) return 0;
	if (*IsDbg == '?') return 1;
	return 0;
}

int ProcessDebugCmd(const char *Cmd) {
	int IsDebug = DetectDebugCommand(Cmd);
	if (IsDebug == 0) return 0;

	Cmd++;

	printf("Recieved command '%s'\n", Cmd);

	return 1;
}
