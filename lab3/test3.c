#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "test3.h"
#include "kbd.h"

#include <stdio.h>

#include <minix/sysutil.h>

#define DELAY_US    20000


int scancode();
int kbd_subscribe_int();
int kbd_unsubscribe_int();
void c_handler();

unsigned int kbd_hook;

int kbd_subscribe_int()
{


	int ret = sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook);
	sys_irqenable(&kbd_hook);
	if(ret < 0)
		return -1;

	//ret = sys_irqsetpolicy(KBD_IRQ, IRQ_EXCLUSIVE, &kbd_hook);

	if(ret < 0)
		return -1;

	return kbd_hook;
}

int kbd_unsubscribe_int()
{
	int ret = sys_irqrmpolicy(&kbd_hook);

	return ret;
}

void c_handler()
{
	int ipc_status;
	message msg;
	unsigned long irq_set;


	int terminus = 0;


	while(terminus == 0)
	{
		printf("1\n");
		irq_set = BIT(kbd_subscribe_int());

		printf("2\n");

		if(driver_receive(ANY, &msg, &ipc_status)!=0)
		{
			printf("Driver_receive failed with");
			continue;
		}
		printf("3\n");

		if(is_ipc_notify(ipc_status))
		{
			printf("4\n");
			switch(_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE:
				if(msg.NOTIFY_ARG & irq_set)
				{
					////////////////////////////
					printf("5\n");
					terminus = scancode();
					///////////////////////////
				}
				break;
			default:
				break;
			}

			printf("6\n");
		}


		tickdelay(micros_to_ticks(DELAY_US));
		kbd_unsubscribe_int();
	}

	printf("OVER\n");
}

int scancode()
{
	unsigned long buf;
	sys_inb(KBD_OUT_BUF, &buf);

	unsigned char mask = BIT(7);

	if((mask && buf) == 0)
		printf("Makecode : %x\n", buf);
	else
		printf("Breakcode : %x\n", buf);

	if(buf == ESC_break)
		return 1;

	return 0;
}

int kbd_test_scan(unsigned short ass)
{
	kbd_hook = 3;

	c_handler();

	return 0;
}


int kbd_test_leds(unsigned short n, unsigned short *leds)
{
    /* To be completed */
}


int kbd_test_timed_scan(unsigned short n)
{
    /* To be completed */
}
