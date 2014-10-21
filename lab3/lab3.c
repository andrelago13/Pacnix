#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <stdio.h>

#include "test3.h"

int main()
{
	sef_startup();

	unsigned short toggle[4] = {2,2,2,2};

	//kbd_test_scan(0);
	kbd_test_leds(4, toggle);

	return 0;
}
