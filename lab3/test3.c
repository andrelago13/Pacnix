#include "test3.h"
#include "kbd.h"

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <stdio.h>

int kbd_hook;

unsigned int kbd_subscribe_int();
unsigned int kbd_unsubscribe_int();
int kbd_int_handler();

int kbd_test_scan(unsigned short ass)
{
	kbd_hook = 3;

	int ipc_status;
	message msg;

	unsigned long irq_set = BIT(kbd_subscribe_int());

	int terminus = 0;

	while( terminus == 0 )
	{
		printf("=================== BEFORE ===================\n");
		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed\n");
			continue;
		}
		printf("=================== AFTER ===================\n");
		if (is_ipc_notify(ipc_status))
		{
			switch (_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set)
				{
					printf("=================== REACHED FUNCTION ===================\n");
					terminus = kbd_int_handler();
				}
				break;
			default:
				break;
			}
		}
	}

	kbd_unsubscribe_int();

}


unsigned int kbd_subscribe_int()
{
	int ret = sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook);

	if (ret < 0)
		return -1;

	return kbd_hook;
}

unsigned int kbd_unsubscribe_int()
{
	int ret = sys_irqrmpolicy(&kbd_hook);

	return ret;
}

int kbd_int_handler()
{
	printf("Reached handler \n");

	return 1;
}



int kbd_test_leds(unsigned short n, unsigned short *leds)
{
    /* To be completed */
}

int kbd_test_timed_scan(unsigned short n)
{
    /* To be completed */
}
