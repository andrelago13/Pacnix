#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "test3.h"

#include <stdio.h>

int main()
{
	sef_startup();

	/* Enable IO-sensitive operations for ourselves */
	//sys_enable_iop(SELF);
	//sys_iopenable(SELF);

	kbd_test_scan(0);


	return 0;
}
