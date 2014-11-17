#include "kbd.h"
#include "timer.h"
#include "i8254.h"

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <stdio.h>
#include <minix/sysutil.h>

int kbd_hook;

int wait_for_esc()
{
	kbd_hook = KBD_IRQ;

	int ipc_status;
	message msg;

	int ret = kbd_subscribe_int();

	if(ret < 0)
		return 1;

	unsigned long irq_set = BIT(ret);

	int terminus = 0;

	while( terminus == 0 )
	{
		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("Driver_receive failed\n");
			continue;
		}
		if (is_ipc_notify(ipc_status))
		{
			switch (_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set)
				{
					unsigned long letra = 0;

					if(OK != sys_inb(KBD_OUT_BUF, &letra))		// Read scancode
						return 1;

					if(letra == ESC_break)
						terminus = 1;
				}
				break;
			default:
				break;
			}
		}
	}

	if(kbd_unsubscribe_int() != 0)
		return 1;

	return 0;
}

int kbd_subscribe_int()
{
	int ret = sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook);

	if (ret < 0)
		return -1;

	return KBD_IRQ;
}

int kbd_unsubscribe_int()
{
	int ret = sys_irqrmpolicy(&kbd_hook);

	if (ret < 0)
		return -1;

	return ret;
}
