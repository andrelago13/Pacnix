#include "test4.h"
#include "kbd.h"

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <stdio.h>
#include <minix/sysutil.h>

int mouse_hook;

// TO-DO //
int test_packet(unsigned short cnt)
{
	set_stream();
	interrupt_cycle(cnt);
	return 0;
}

int kbd_mouse_subscribe_int()
{
	int ret = sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook);

	if (ret < 0)
		return -1;

	return MOUSE_IRQ;
}

int kbd_mouse_unsubscribe_int()
{
	int ret = sys_irqrmpolicy(&mouse_hook);

	if (ret < 0)
		return -1;

	return ret;
}

int interrupt_cycle(int packets)
{
	mouse_hook = MOUSE_IRQ;

	int ipc_status;
	message msg;

	int ret = kbd_mouse_subscribe_int();

	if(ret < 0)
		return 1;

	unsigned long irq_set = BIT(ret);

	int terminus = packets;

	while(terminus != 0)
	{
		printf("===\n");
		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("Driver_receive failed\n");
			continue;
		}
		printf("###\n");
		if (is_ipc_notify(ipc_status))
		{
			switch (_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set)
				{
					int decrement; //= mouse_handler();
					printf("Hello222\n");
					unsigned long test;
					sys_inb(KBD_OUT_BUF, &test);
					terminus--;
					//terminus -= decrement;
				}
				break;
			default:
				break;
			}
		}
	}

	if(kbd_mouse_unsubscribe_int() != 0)
		return 1;

	return 0;
}

int mouse_handler()
{
	static long unsigned int byte1, byte2, byte3;
	if(byte1 == 0)
	{
		byte2 = 0;
		byte3 = 0;

		if(OK != sys_inb(KBD_OUT_BUF, &byte1))
			return 1;
	}


}

void set_stream()
{
	unsigned long stat;

	while(1)
	{
		sys_inb(STAT_REG, &stat);

		if((stat & IBF) == 0)
		{
			sys_outb(KBC_CMD_REG, DISABLE_MOUSE);
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}

	while(1)
	{
		sys_inb(STAT_REG, &stat);

		if((stat & IBF) == 0)
		{
			sys_outb(KBC_CMD_REG, WRITE_MOUSE);
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}

	tickdelay(micros_to_ticks(DELAY_US));

	while(1)
	{
		sys_inb(STAT_REG, &stat);

		if((stat & IBF) == 0)
		{
			sys_outb(KBC_CMD_REG, SET_STREAM_MOD);
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}

	while(1)
	{
		sys_inb(STAT_REG, &stat);

		if((stat & IBF) == 0)
		{
			sys_outb(KBC_CMD_REG, ENABLE_MOUSE);
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}

}




// TO-DO //
int test_async(unsigned short idle_time)
{
    /* To be completed ...*/
}

// TO-DO //
int test_config(void)
{
    /* To be completed ...*/
}

// TO-DO //
int test_gesture(short length, unsigned short tolerance)
{

}
