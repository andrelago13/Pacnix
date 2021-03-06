#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/video.h>
#include <sys/mman.h>

#include "videobe.h"
#include "video.h"
#include "video_gr.h"
#include "sprite.h"
#include "timer.h"
#include "i8254.h"
#include "mouse.h"

static char *video_mem;		/* Address to which VRAM is mapped */

static unsigned scr_width;	/* Width of screen in columns */
static unsigned scr_lines;	/* Height of screen in lines */


#define VRAM_PHYS_ADDR	0xF0000000
#define H_RES             1024
#define V_RES		  768
#define BITS_PER_PIXEL	  8

static char *video_mem;		/* Process address to which VRAM is mapped */

static char *video_mem_buffer;	/* Address to which temporary VRAM buffer is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */



int vg_exit() {
  struct reg86u reg86;

  reg86.u.b.intno = 0x10; /* BIOS video services */

  reg86.u.b.ah = 0x00;    /* Set Video Mode function */
  reg86.u.b.al = 0x03;    /* 80x25 text mode*/

  if( sys_int86(&reg86) != OK ) {
      printf("\tvg_exit(): sys_int86() failed \n");
      return 1;
  } else
      return 0;
}

void *vg_init(unsigned short mode)
{
	/////////////////////////////////////////////////////////////////
	// Set graphics mode ////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////

	struct reg86u reg86;

	reg86.u.b.intno = IV_VEC_VIDEOCARD;	// Select BIOS video card services
	reg86.u.b.ah = VBE_FUNCT;
	reg86.u.b.al = FUNC_SET_VBE_MODE;
	reg86.u.w.bx = (LINEAR_FRAME_BUF | mode) & CLEAR_DISPLAY_MEM;

	if( sys_int86(&reg86) != OK )
	{
		printf("\tvg_init(): sys_int86() failed \n");
		return NULL;
	}
	else if(reg86.u.b.ah == REQ_MODE_NOT_AVAILABLE)
	{
		printf("\tvg_init(): requested mode not available");
		return NULL;
	}

	vbe_mode_info_t *vmode_info_p;

	vmode_info_p = malloc(sizeof(vbe_mode_info_t));

	if(vbe_get_mode_info(mode, vmode_info_p) != 0)
	{
		printf("\tvg_init(): vbe_get_mode_info() failed \n");
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////
	// Map virtual memory to VRAM, based in lab1 /////////////////////////
	//////////////////////////////////////////////////////////////////////

	int r;
	struct mem_range mr;

	/* Allow memory mapping */

	mr.mr_base = (phys_bytes)(vmode_info_p->PhysBasePtr);
	mr.mr_limit = mr.mr_base + (vmode_info_p->XResolution * vmode_info_p->YResolution * vmode_info_p->BitsPerPixel);

	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
	{
		free(vmode_info_p);
		panic("\tvg_init(): sys_privctl (ADD_MEM) failed: %d\n", r);
		return NULL;
	}

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, (vmode_info_p->XResolution * vmode_info_p->YResolution * vmode_info_p->BitsPerPixel));

	video_mem_buffer = malloc((vmode_info_p->XResolution * vmode_info_p->YResolution * vmode_info_p->BitsPerPixel/8));

	if(video_mem == MAP_FAILED)
	{
		free(vmode_info_p);
		panic("vg_init couldn't map video memory");
		return NULL;
	}

	/* Save text mode resolution */

	v_res = vmode_info_p->YResolution;
	h_res = vmode_info_p->XResolution;
	bits_per_pixel = vmode_info_p->BitsPerPixel;

	free(vmode_info_p);
	return video_mem;
}

int draw_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color)
{
	if((x >= h_res) || (y >= v_res) || (x+size >= h_res) || (y+size >= v_res))
		return 1;

	unsigned short i;
	for(i = x; i < x+size; i++)
	{
		paint_pixel(i, y, color);
	}

	for(i = y; i < y+size; i++)
	{
		unsigned short j;
		for(j = x; j < x+size; j++)
		{
			paint_pixel(j, i, color);
		}
	}

	for(i = x; i < x+size; i++)
	{
		paint_pixel(i, y+size-1, color);
	}

	return 0;
}

int paint_pixel(unsigned short x, unsigned short y, unsigned long color)
{
	if(color == COLOR_TRANSPARENT)
		return 0;

	if(x >= h_res || y >= v_res)
		return 1;

	char *coord = video_mem_buffer + x*bits_per_pixel/8 + h_res*y*bits_per_pixel/8;
	*coord = (char) color;

	return 0;
}

int draw_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color)
{
	if((xi >= h_res) || (yi >= v_res) || (xf >= h_res) || (yf >= v_res))
		return 1;

	if(xf == xi)		// Vertical line //
	{
		int i;

		if(yf < yi)
		{
			unsigned short temp = yi;
			yi = yf;
			yf = temp;
		}

		for(i = yi; i <= yf; i++)
		{
			paint_pixel(xi, i, color);
		}

		return 0;
	}

	/////////////////////
	if(xf < xi)
	{
		unsigned short temp = xi;
		xi = xf;
		xf = temp;
		temp = yi;
		yi = yf;
		yf = temp;
	}
	/////////////////////   Make xi hold the smaller value, for simplification

	float slope = (float)(yf-yi)/(xf-xi);

	if(slope == 0)		// Horizontal line //
	{
		int i;
		for(i = xi; i <= xf; i++)
		{
			paint_pixel(i, yi, color);
		}
	}
	else				// Non horizontal nor vertical line //
	{
		float intercept = (float)yi-slope*xi;
		int i;

		if(abs(slope) > 1)		// x = f(y) => make sure all points are painted
		{
			if(yf < yi)
			{
				for(i = yf; i <= yi; i++)
				{
					unsigned short x = (i-intercept)/slope;
					paint_pixel(x, i, color);
				}
			}
			else
			{
				for(i = yi; i <= yf; i++)
				{
					unsigned short x = (i-intercept)/slope;
					paint_pixel(x, i, color);
				}
			}
		}
		else				// y = f(x) => make sure all points are painted
		{
			for(i = xi; i <= xf; i++)
			{
				unsigned short y = slope*i+intercept;
				paint_pixel(i, y, color);
			}
		}

		return 0;
	}

	return 0;
}

int draw_img(Sprite *img)
{
	if(img == NULL)
		return 1;

	if(img->x >= h_res || img->y >= v_res)
		return 1;

	int i, j;
	int ret = 0;
	char *pix = img->map;

	for(i = img->y; i < img->height+img->y; i++)
	{
		for(j = img->x; j < img->width+img->x; j++)
		{
			if(0 != paint_pixel(j, i, *pix))
				ret = 1;
			pix += bits_per_pixel/8;
		}
	}

	return ret;
}

void draw_back_map(char *map)
{
	memcpy(video_mem_buffer, map, h_res*v_res*bits_per_pixel/8);
}

void fill_screen(unsigned long color)
{
	memset(video_mem_buffer, (char) color, h_res*v_res*bits_per_pixel/8);
}

int delete_img(Sprite *img)
{
	if(img->x >= h_res || img->y >= v_res)
		return 1;

	int i, j;

	for(i = img->y; i < img->height+img->y; i++)
	{
		for(j = img->x; j < img->width+img->x; j++)
		{
			paint_pixel(j, i, (char)0);
		}
	}

	return 0;
}

int move_img(Sprite_move *img)
{
	if(img->sp->x >= h_res || img->sp->y >= v_res)
		return 1;

	unsigned int t0_hook = TIMER0_IRQ;

	int ipc_status;
	message msg;
	unsigned long irq_set;

	irq_set = BIT(timer_subscribe(&t0_hook));
	int terminus = 0;

	Sprite to_delete;
	to_delete.width = img->sp->width;
	to_delete.height = img->sp->height;

	double x_dbl, y_dbl;	// records the double values of x and y of the image
	x_dbl = img->sp->x;			// so that the interrupt cycle rounds them
	y_dbl = img->sp->y;			// to match pixels

	while(terminus == 0)
	{
		if(driver_receive(ANY, &msg, &ipc_status)!=0)
		{
			printf("Driver_receive failed with");
			continue;
		}

		if(is_ipc_notify(ipc_status))
		{
			switch(_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE:
				if(msg.NOTIFY_ARG & irq_set)
				{
					delete_img(&to_delete);
					if(draw_img(img->sp) != 0)
						terminus = 1;
					to_delete.x = img->sp->x;
					to_delete.y = img->sp->y;

					x_dbl += img->xspeed;
					y_dbl += img->yspeed;

					img->sp->x = x_dbl;
					img->sp->y = y_dbl;

					//if((abs(img->x_to_reach - img->x) < 1) || (abs(img->y_to_reach - img->y) < 1))
					//	terminus = 1;

					if((((to_delete.x <= img->x_to_reach) & (img->sp->x >= img->x_to_reach)) || ((to_delete.x >= img->x_to_reach) & (img->sp->x <= img->x_to_reach))) && (img->x_to_reach != -1))
						terminus = 1;

					if((((to_delete.y <= img->y_to_reach) & (img->sp->y >= img->y_to_reach)) || ((to_delete.y >= img->y_to_reach) & (img->sp->y <= img->y_to_reach))) && (img->y_to_reach != -1))
						terminus = 1;

				}
				break;
			default:
				break;
			}
		}
	}

	timer_unsubscribe(&t0_hook);

	return 0;
}

int test_controller_config()
{
	vbe_info_t *vbe_p;

	vbe_p = malloc(sizeof(vbe_info_t));

	int16_t *modes = vbe_get_info(vbe_p);

	if(modes == NULL)
	{
		free(vbe_p);
		printf("ERROR : vbe_get_info() failed unexpectadly\n\n");
		return 1;
	}

	// Capabilities

	if((vbe_p->capabilities[0] & BIT(0)) == 0)
		printf("DAC is fixed width, with 6 bits per primary color\n");
	else
		printf("DAC width is switchable to 8 bits per primary color\n");

	if((vbe_p->capabilities[0] & BIT(1)) == 0)
		printf("Controller is VGA compatible\n");
	else
		printf("Controller is not VGA compatible\n");

	if((vbe_p->capabilities[0] & BIT(2)) == 0)
		printf("Normal RAMDAC operation\n\n");
	else
		printf("When programming large blocks of information to the RAMDAC, use the blank bit in Function 09h\n\n");


	// List of mode numbers

	int16_t *mode_ptr = modes;

	int i = 1;

	printf("List of available VBE modes:\n");

	while((unsigned short)*mode_ptr != LAST_VBE_MODE)
	{
		printf(" %u -> 0x%X\n", i, *mode_ptr);
		mode_ptr += 2;
		i++;
	}
	printf(" -> List ended\n\n");

	// Size of VRAM memory

	unsigned short vram_size = vbe_p->totalMemory;
	printf("Size of VRAM memory : %u KB (%u 64KB units)\n\n", vram_size, vram_size/64);

	free(vbe_p);

	return 0;
}

void draw_mouse(Mouse_coord *mouse)
{
	draw_img(&mouse->img);
}

void erase_mouse(Mouse_coord *mouse)
{
	delete_img(&mouse->img);
}

void update_buffer()
{
	memcpy(video_mem, video_mem_buffer, h_res*v_res*bits_per_pixel/8);
}

char * pixel_vid(unsigned int x, unsigned int y)
{
	if(x >= h_res)
		x = h_res;

	if(y >= v_res)
		y = v_res;

	char *coord = video_mem_buffer + x*bits_per_pixel/8 + h_res*y*bits_per_pixel/8;
	return coord;
}

int draw_rectangle(int xi, int yi, int xf, int yf, unsigned long color)
{
	int width = xf-xi;
	int height = yf-yi;

	int x, y; x = xi; y = yi;

	int i = 0;
	for(;i<width;i++)
	{
		paint_pixel(x, y, color);
		x++;
	}

	i = 0;
	x = xi;
	y++;
	for(;i<(height-1);i++)
	{
		paint_pixel(x, y, color);
		paint_pixel(x+width-1, y, color);
		y++;
	}

	i=0;
	x = xi;
	y = yi + height - 1;
	for(;i<width;i++)
	{
		paint_pixel(x, y, color);
		x++;
	}

	return 0;
}

int is_in_img(Sprite * sp, int x, int y)
{
	if((x >= sp->x) && (x < sp->x + sp->width))
	{
		if((y >= sp->y) && (y < sp->y + sp->height))
		{
			return 1;
		}
	}

	return 0;
}

void surround_img(Sprite * sp, unsigned long color)
{
	draw_rectangle(sp->x, sp->y, sp->x + sp->width, sp->y + sp->height, color);
}

void print_num(Sprite * sp, unsigned long color)
{
	if(sp->x >= h_res || sp->y >= v_res)
		return;

	int i, j;
	char *pix = sp->map;

	for(i = sp->y; i < sp->height+sp->y; i++)
	{
		for(j = sp->x; j < sp->width+sp->x; j++)
		{
			if(*pix == 0)
			{
				paint_pixel(j, i, color);
			}

			pix += bits_per_pixel/8;
		}
	}
}

