#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <stdio.h>

#include "kbd.h"
#include "test4.h"


int main(int argc, char *argv[])
{
	sef_startup();

	test_packet(5);
	return 0;
}

