#include "timer.h"
#include "i8254.h"
#include <minix/drivers.h>

#include <stdio.h>

int main()
{
	sef_startup();

	printf("\n   ==== Test functions start here ==== \n \n");
	printf(" ==> Testing timer_get_conf() \n ARGS(0, memory)\n\n");

	unsigned char *rst;
	rst = malloc(sizeof(unsigned char));

	//////////////////////////////
	//sys_outb(TIMER_CTRL, 0x16);
	sys_outb(TIMER_CTRL, 0x16);
	sys_outb(TIMER_0, 0x11);
	printf("Control: 0x16 - 00010110");
	/////////////////////////////////

	timer_get_conf(0, rst);

	printf("\n \n Result:  rst = 0x%p\n         *rst = 0x%x\n\n\n ==> Testing timer_display_conf()\n\n", rst, *rst);

	timer_display_conf(*rst);

	free(rst);

	return 0;
}
