#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <stdio.h>

#include "test3.h"

int main()
{
	sef_startup();

	kbd_test_scan(0);

	return 0;
}
