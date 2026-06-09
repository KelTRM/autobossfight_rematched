#include"../ui/console_manager/console.h"

#ifdef __linux__
#include<unistd.h>
#endif

int test() {
	BUFHANDLE Buffer = CreateBuffer();

	SwitchBuffer(Buffer);
//	ActiveBuffer = Buffer;

	bprintf(Buffer, "Test1\n");
	bprintf(Buffer, "Test2\n");

	sleep(3);

	BUFHANDLE Buffer2 = CreateBuffer();

	bprintf(Buffer2, "Tst1\n");
	bprintf(Buffer2, "Tst2\n");

	bprintf(Buffer2, "Tst3\n");

	SwitchBuffer(Buffer2);

	sleep(3);

	SwitchBuffer(Buffer);

	sleep(3);

	SwitchBuffer(Buffer2);

	return 0;
}
