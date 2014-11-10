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

	sys_irqenable(&mouse_hook);

	if (ret < 0)
		return -1;

	return MOUSE_IRQ;
}

int kbd_mouse_unsubscribe_int()
{
	int ret = sys_irqrmpolicy(&mouse_hook);

	sys_irqdisable(&mouse_hook);

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
					int decrement = mouse_handler();
					terminus = terminus - decrement;
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
	static int counter;
	static unsigned char packet[3];

	unsigned long byte;
	sys_inb(KBD_OUT_BUF, &byte);

	if(counter !=2 && counter != 3)
	{
		packet[0] = 0;
		packet[1] = 0;
		packet[2] = 0;

		packet[0] = byte;


		if((packet[0] & PACKET_BYTE_1) == 0)
		{
			packet[0]=0;
			counter = 1;
			return 0;
		}

		counter = 2;
		return 0;
	}

	if(counter == 2)
	{
		packet[1] = byte;
		counter = 3;
		return 0;
	}

	if(counter == 3)
	{
		packet[2] = byte;
		counter = 1;

		unsigned int mb, lb, rb, x_delta, y_sign, x_ovf, y_ovf;
		long int y_delta, x_sign;

		x_delta = packet[1];
		y_delta = packet[2];

		if((packet[0] & Y_OVF) == 0)
			y_ovf = 0;
		else
			y_ovf = 1;

		if((packet[0] & X_OVF) == 0)
			x_ovf = 0;
		else
			x_ovf = 1;

		if((packet[0] & Y_SIGN) != 0)
			y_delta = y_delta | NEGATIVE;
		else
			y_delta = y_delta & POSITIVE;

		if((packet[0] & X_SIGN) != 0)
			x_delta = x_delta | NEGATIVE;
		else
			x_delta = x_delta & POSITIVE;

		if((packet[0] & MB) == 0)
			mb = 0;
		else
			mb = 1;

		if((packet[0] & LB) == 0)
			lb = 0;
		else
			lb = 1;

		if((packet[0] & RB) == 0)
			rb = 0;
		else
			rb = 1;

		printf("B1=0x%X	B2=0x%X	B3=0x%X	LB=%u	MB=%u	RB=%u	XOV=%u	YOV=%u	X=%d	Y=%d\n", packet[0], packet[1],
				packet[2], lb, mb, rb, x_ovf, y_ovf, x_delta, y_delta);

		return 1;
	}

	return 0;
}

void set_stream()
{
	unsigned long stat;

	while(1)
	{
		sys_inb(STAT_REG, &stat);

		if((stat & IBF) == 0)
		{
			sys_outb(KBC_CMD_REG, WRITE_MOUSE);
			printf("Write to mouse\n");
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
			sys_outb(KBD_IN_BUF, SET_STREAM_MOD);
			printf("Enable stream mode\n");
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}

	tickdelay(micros_to_ticks(DELAY_US));

	sys_inb(KBD_OUT_BUF, &stat);

	while(1)
	{
		sys_inb(STAT_REG, &stat);

		if((stat & IBF) == 0)
		{
			sys_outb(KBC_CMD_REG, WRITE_MOUSE);
			printf("Write to mouse\n");
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
			sys_outb(KBD_IN_BUF, EN_DATA_REP);
			printf("Enable stream mode\n");
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}


	tickdelay(micros_to_ticks(DELAY_US));

	sys_inb(KBD_OUT_BUF, &stat);

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
