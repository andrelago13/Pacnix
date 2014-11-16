#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <stdio.h>
#include <minix/sysutil.h>

#include <machine/int86.h>
#include "lmlib.h"
#include <stdint.h>

#include "test5.h"
#include "video_gr.h"
#include "timer.h"
#include "vbe.h"

void *test_init(unsigned short mode, unsigned short delay) {
	
	char *video_mem = vg_init(mode);

	if(video_mem == NULL)
	{
		printf("\ttest_init(): vg_init() failed\n");
		vg_exit();
		return NULL;
	}

	wait_x_sec(delay);
	vg_exit();

	vbe_mode_info_t vmode_info_p;

	if(vbe_get_mode_info(mode, &vmode_info_p) != 1)
	{
		printf("\ttest_init(): vbe_get_mode_info() failed \n");
		return NULL;
	}

	printf(" Physical address of VRAM : 0x%X \n\n", vmode_info_p.PhysBasePtr);

	return video_mem;
}


int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	
	/* To be completed */
	
}

int test_line(unsigned short xi, unsigned short yi, 
		           unsigned short xf, unsigned short yf, unsigned long color) {
	
	/* To be completed */
	
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	
	/* To be completed */
	
}	

int test_move(unsigned short xi, unsigned short yi, char *xpm[], 
				unsigned short hor, short delta, unsigned short time) {
	
	/* To be completed */
	
}					


