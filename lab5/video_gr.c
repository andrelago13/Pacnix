#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/video.h>
#include <sys/mman.h>

#include "vbe.h"
#include "video.h"

static char *video_mem;		/* Address to which VRAM is mapped */

static unsigned scr_width;	/* Width of screen in columns */
static unsigned scr_lines;	/* Height of screen in lines */

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR	0xF0000000
#define H_RES             1024
#define V_RES		  768
#define BITS_PER_PIXEL	  8

/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */

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
	reg86.u.w.bx = LINEAR_FRAME_BUF | mode;

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
		panic("\tvg_init(): sys_privctl (ADD_MEM) failed: %d\n", r);
		return NULL;
	}

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, (vmode_info_p->XResolution * vmode_info_p->YResolution * vmode_info_p->BitsPerPixel));

	if(video_mem == MAP_FAILED)
	{
		panic("vg_init couldn't map video memory");
		return NULL;
	}

	/* Save text mode resolution */

	v_res = vmode_info_p->YResolution;
	h_res = vmode_info_p->XResolution;
	bits_per_pixel = vmode_info_p->BitsPerPixel;

	return video_mem;
}

void draw_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color)
{
	char *video_mem = vg_init(0x105);
	char *x_coord = video_mem + x*bits_per_pixel/8 + h_res*y*bits_per_pixel/8;
	char *temp = x_coord;

	int i;
	for(i = 0; i < size; i++)
	{
		*temp = (char)color;
		temp += 1;
	}

	temp = x_coord + h_res*y*bits_per_pixel/8;

	for(i = 0; i < size; i++)
	{
		*temp = (char)color;
		temp += size - 1;
		*temp = (char)color;
		temp -= size - 1;
		temp += h_res*y*bits_per_pixel/8;
	}

	temp = x_coord + size*h_res*y*bits_per_pixel/8;
	for(i = 0; i < size; i++)
	{
		*temp = (char)color;
		temp += 1;
	}
}




