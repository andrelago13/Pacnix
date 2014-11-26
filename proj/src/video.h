#ifndef __VIDEO_H
#define __VIDEO_H

#include "i8254.h"


// BIOS Services interrupt vectors
#define IV_VEC_VIDEOCARD 0x10
#define IV_VEC_PCCONFIG 0x11
#define IV_VEC_MEMCONFIG 0x12
#define IV_VEC_KEYBOARD 0x16

#define VBE_FUNCT 0x4F			// must be 0x4F..
#define FUNC_RET_VBE_INFO 0x00
#define FUNC_RET_VBE_MODE_INFO 0x01
#define FUNC_SET_VBE_MODE 0x02

#define LINEAR_FRAME_BUF BIT(14)
#define CLEAR_DISPLAY_MEM ~BIT(15)

#define REQ_MODE_NOT_AVAILABLE 0x01

#define GRAF_1024x768 0x105


#define HRES             1024
#define VRES		  768
#define BITS_PER_PIXEL	  8

#define LAST_VBE_MODE 0xFFFF

#define TWO_BYTE_ADDR 0xFFFF0000
#define ADDR_OFFSET 0x0FFFF
#define FIRST_32BITE_F 0xF0000000


// Minix colors
#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_BROWN 6
#define COLOR_LIGHTGRAY 7
#define COLOR_DARKGRAY 8
#define COLOR_LIGHTBLUE 9
#define COLOR_LIGHTGREEN 10
#define COLOR_LIGHTCYAN 11
#define COLOR_LIGHTRED 12
#define COLOR_LIGHTMAGENTA 13
#define COLOR_YELLOW 14
#define COLOR_WHITE 15

// Chosen this value for "transparent" color
#define COLOR_TRANSPARENT 55

#endif /* __VIDEO__H */
