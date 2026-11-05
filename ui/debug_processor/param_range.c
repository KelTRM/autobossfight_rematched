#include"../../debug/debug.h"

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

