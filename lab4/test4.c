#include "test4.h"
#include "kbd.h"

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <stdio.h>

int kbd_hook;

int kbd_mouse_subscribe_int()
{
	int ret = sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook); // não tenho a certeza se aqui é kbd_kbd

	if (ret < 0)
		return -1;

	return MOUSE_IRQ;
}

int kbd_mouse_unsubscribe_int()
{
	int ret = sys_irqrmpolicy(&kbd_hook);

	if (ret < 0)
		return -1;

	return ret;
}
int test_packet(unsigned short cnt)
{
    /* To be completed ... */
}

int test_async(unsigned short idle_time)
{
    /* To be completed ... */
}

int test_config(void)
{
    /* To be completed ... */
}

int test_gesture(short length, unsigned short tolerance)
{
    /* To be completed ... */
}
