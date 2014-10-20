#include "test3.h"
#include "kbd.h"
#include "timer.h"

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <stdio.h>
#include <minix/sysutil.h>

#define DELAY_US    20000

int kbd_hook;

unsigned int kbd_subscribe_int();
unsigned int kbd_unsubscribe_int();
int kbd_int_handler();

int kbd_test_scan(unsigned short ass)
{
	kbd_hook = KBD_IRQ;

	int ipc_status;
	message msg;

	unsigned long irq_set = BIT(kbd_subscribe_int());

	int terminus = 0;

	while( terminus == 0 )
	{
		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed\n");
			continue;
		}
		if (is_ipc_notify(ipc_status))
		{
			switch (_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set)
				{
					terminus = kbd_int_handler();
				}
				break;
			default:
				break;
			}
		}
	}

	kbd_unsubscribe_int();

	return 0;

}


unsigned int kbd_subscribe_int()
{
	int ret = sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook);

	if (ret < 0)
		return -1;

	return KBD_IRQ;
}

unsigned int kbd_unsubscribe_int()
{
	int ret = sys_irqrmpolicy(&kbd_hook);

	return ret;
}

int kbd_int_handler()
{
	unsigned long letra = 0;

	sys_inb(KBD_OUT_BUF, &letra);

	if((letra & BIT(7)) != 0)
		printf("Break code : 0x%x\n", letra);
	else
		printf("Make code : 0x%x\n", letra);

	if(letra == 0x81)
		return 1;

	return 0;
}



int kbd_test_leds(unsigned short n, unsigned short *leds)
{
	unsigned int i = 0;

	while(i < n)
	{
		unsigned long led_cmd = 0;

		sys_inb(KBD_OUT_BUF, &led_cmd);

		led_cmd = led_cmd ^ BIT(leds[i]);

		switch(leds[i])
		{
		case 0:
			printf("Toggled scroll lock\n");
		case 1:
			printf("Toggled numeric lock\n");
		case 2:
			printf("Toggled caps lock\n");
		}

		sys_outb(KBD_OUT_BUF, SET_RESET_CMD);
		tickdelay(micros_to_ticks(DELAY_US));
		sys_outb(KBD_OUT_BUF, led_cmd);

		wait_1_sec();

		i++;
	}

	return 0;
}

int kbd_test_timed_scan(unsigned short n)
{
    /* To be completed */
}
