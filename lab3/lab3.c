#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "test3.h"

int main()
{
	sef_startup();

	/* Enable IO-sensitive operations for ourselves */
	//sys_iopenable(SELF);




	return 0;
}
