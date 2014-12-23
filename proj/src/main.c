#include "pacnix.h"
#include "video.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	time_t t;
	srand((unsigned) time(&t));
	sef_startup();

//	vg_init(0x105);
//	draw_rectangle(5, 5, 200, 200, 1);
//	update_buffer();
//	wait_for_esc();
//	vg_exit();
//	printf("HELLO\n\n");
//	return 0;

	vg_exit();	// to prevent "darker colors" on first run
	vg_init(GRAF_1024x768);
	pacnix_start();
	vg_exit();
	printf("\n\n\tProgram ended\n\n\tThanks for playing!\n\n");

	return 0;
}
