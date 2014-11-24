#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"
#include "video.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

	struct reg86u reg86;

	reg86.u.b.intno = IV_VEC_VIDEOCARD;	// Select BIOS video card services
	reg86.u.b.ah = VBE_FUNCT;
	reg86.u.b.al = FUNC_RET_VBE_MODE_INFO;
	reg86.u.w.cx = mode;

	if(lm_init() != 0)
		return 1;

	mmap_t map;

	lm_alloc(sizeof(vbe_mode_info_t), &map);

	reg86.u.w.es = PB2BASE(map.phys);
	reg86.u.w.di = PB2OFF(map.phys);

	if( sys_int86(&reg86) != OK )
	{
		printf("\tvbe_get_mode_info(): sys_int86() failed \n");
		return 1;
	}

	*vmi_p = *((vbe_mode_info_t *)map.virtual);

	lm_free(&map);

	return 0;
}

int16_t *vbe_get_info(vbe_info_t *vbe_p)
{
	vbe_p->vbeSignature[0] = '2';
	vbe_p->vbeSignature[1] = 'E';
	vbe_p->vbeSignature[2] = 'B';
	vbe_p->vbeSignature[3] = 'V';

	struct reg86u reg86;

	reg86.u.b.intno = IV_VEC_VIDEOCARD;	// Select BIOS video card services
	reg86.u.b.ah = VBE_FUNCT;
	reg86.u.b.al = FUNC_RET_VBE_INFO;

	if(lm_init() != 0)
		return NULL;

	mmap_t map;

	lm_alloc(sizeof(vbe_info_t), &map);

	reg86.u.w.es = PB2BASE(map.phys);
	reg86.u.w.di = PB2OFF(map.phys);

	if( sys_int86(&reg86) != OK )
	{
		printf("\tvbe_get_mode_info(): sys_int86() failed \n");
		return NULL;
	}

	*vbe_p = *((vbe_info_t *)map.virtual);

	void *modes_ptr = (void *)(((vbe_p->videoModePtr & TWO_BYTE_ADDR) >> 12) + (vbe_p->videoModePtr & 0x0FFFF) + ((uint32_t)map.virtual & FIRST_32BITE_F));

	lm_free(&map);

	int16_t *modes = modes_ptr;

	return modes;
}

