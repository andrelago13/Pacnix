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
#include "kbd.h"
#include "read_xpm.h"
#include "video.h"
#include "sprite.h"

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

	if(vbe_get_mode_info(mode, &vmode_info_p) != 0)
	{
		printf("\ttest_init(): vbe_get_mode_info() failed \n");
		return NULL;
	}

	printf(" Physical address of VRAM : 0x%X \n\n", vmode_info_p.PhysBasePtr);

	return video_mem;
}


int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	
	if(draw_square(x, y, size, color) != 0)
	{
		vg_exit();
		printf("\tERROR : draw_square() failed\n\tInvalid combination of coordinates and size for this screen type\n");
		return 1;
	}

	wait_for_esc();
	vg_exit();
	printf("\n\tProgram ended - user released ESC\n\n");
	return 0;
}

int test_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color) {

	if(draw_line(xi, yi, xf, yf, color) == 1)
	{
		vg_exit();
		printf("\tERROR : draw_line() failed\n\tInvalid combination of coordinates for this screen type\n");
		return 1;
	}

	wait_for_esc();
	vg_exit();
	printf("\n\tProgram ended - user released ESC\n\n");
	return 0;
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	
	Sprite img;

	img.map = read_xpm(xpm, &img.width, &img.height);
	img.x = xi;
	img.y = yi;

	vg_init(GRAF_1024x768);
	if(draw_img(&img) != 0)
	{
		vg_exit();
		printf("\n\t Invalid picture coordinates\n\n");
		return 1;
	}

	wait_for_esc();
	vg_exit();
	printf("\n\tProgram ended - user released ESC\n\n");
	return 0;


	
}	

int test_move(unsigned short xi, unsigned short yi, char *xpm[], 
				unsigned short hor, short delta, unsigned short time) {
	
	/* To be completed */
	
}					


