#include <minix/drivers.h>
#include <sys/video.h>
#include <sys/mman.h>

#include <assert.h>

#include "vt_info.h"

#include "vmem_txt.h"

/* Private global variables */

static char *video_mem;		/* Address to which VRAM is mapped */

static unsigned scr_width;	/* Width of screen in columns */
static unsigned scr_lines;	/* Height of screen in lines */

void vt_fill(char ch, char attr) {


  char *screen_ptr;
  screen_ptr = video_mem;

  int i = 0;

  for(i = 0; i < 2000; i++)
  {
	  *screen_ptr = ch;
	  screen_ptr++;
	  *screen_ptr = attr;
	  screen_ptr++;
  }

}

void vt_blank() {

	vt_fill(0x0000, 0x0000);


}

int vt_print_char(char ch, char attr, int r, int c) {

  char *position;

  if(r>=scr_lines || c >= scr_width || r < 0 || c < 0)
	  return -1;

  position = video_mem + scr_width*2*r + 2*c;

  *position = ch;
  position++;
  *position = attr;

  return 0;

}

int vt_print_string(char *str, char attr, int r, int c) {

  if (r>=scr_lines || c >= scr_width || r < 0 || c < 0)
	  return -1;

  while(*str!=0)
  {
	  int res = vt_print_char(*str, attr, r, c);

	  if (res != 0)
		  return -1;

	  c++;

	  if(c==scr_width)
	  {
		  c=0;
		  r++;
	  }

	  str++;
  }

  return 0;

}

int vt_print_int(int num, char attr, int r, int c) {

  if (r>=scr_lines || c >= scr_width || r < 0 || c < 0)
	  return -1;

  int digits = 1;
  int temp = num;

  if(temp != 0)
	  while(temp!=0)
	  {
		  temp = temp/10;
		  digits++;
	  }

  char *ptr;

  ptr = malloc(digits+1);

  char *letra = ptr + digits; // + 1
  *letra = 0;
  letra--;
  letra--;

  int divisor = 10;

  while (digits>0)
  {
	  *letra = num%divisor+0x0030;
	  num = num/divisor;
	  letra--;
	  digits--;
  }

  int res = vt_print_string(ptr, attr, r, c);

  free(ptr);

  return res;
}

int vt_draw_frame(int width, int height, char attr, int r, int c) {

	// 0x2502 - vertical line
	// 0x2500 - horizontal line
	// 0x2510 - top right corner
	// 0x2514 - bottom left corner
	// 0x2518 - bottom right corner
	// 0x250c - top left corner

	if(r>=scr_lines || c >= scr_width || r < 0 || c < 0)
		return -1;

	vt_print_char(UL_CORNER, attr, r, c);

	int column;

	for(column = width-2; column>0; column--)
		vt_print_char(HOR_BAR, attr, r, c+column);

	vt_print_char(UR_CORNER, attr, r, c+width-1);

	int lines;

	for(lines = height - 2; lines > 0; lines--)
	{
		vt_print_char(VERT_BAR, attr, r+lines, c);
		vt_print_char(VERT_BAR, attr, r+lines, c+width-1);
	}

	vt_print_char(LL_CORNER, attr, r+height-1, c);

	for(column = width-2; column>0; column--)
		vt_print_char(HOR_BAR, attr, r+height-1, c+column);

	vt_print_char(LR_CORNER, attr, r+height-1, c+width-1);

	return 0;
}

char *vt_init(vt_info_t *vi_p) {

  int r;
  struct mem_range mr;

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes)(vi_p->vram_base);
  mr.mr_limit = mr.mr_base + vi_p->vram_size;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
	  panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vi_p->vram_size);

  if(video_mem == MAP_FAILED)
	  panic("video_txt couldn't map video memory");

  /* Save text mode resolution */

  scr_lines = vi_p->scr_lines;
  scr_width = vi_p->scr_width;

  return video_mem;
}
