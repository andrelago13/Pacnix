#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <stdio.h>
#include <minix/sysutil.h>
#include <stdint.h>

#include "mouse.h"
#include "mouse_header.h"
#include "kbd_header.h"
#include "i8254.h"
#include "pixmaps.h"


int mouse_hook, tmr_hook, counter;
unsigned char packet[3];


// TO-CORRECT //
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

int mouse_subscribe(unsigned int *hook)
{
	int ret = sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, hook);

	sys_irqenable(hook);

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

int mouse_unsubscribe(unsigned int *hook)
{
	int ret = sys_irqrmpolicy(hook);

	sys_irqdisable(hook);

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
					unsigned long byte;
					sys_inb(KBD_OUT_BUF, &byte);
				}
				break;
			default:
				break;
			}
		}
	}
	unsigned long byte;
	sys_inb(KBD_OUT_BUF, &byte);
	tickdelay(micros_to_ticks(DELAY_US));
	sys_inb(KBD_OUT_BUF, &byte);
	printf("\n\tDone\n");
	if(kbd_mouse_unsubscribe_int() != 0)
		return 1;

	return 0;
}

int mouse_handler()
{
	static unsigned char packet[3];

		unsigned long byte;
		byte = 0;
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


			unsigned int mb, lb, rb, x_sign, y_sign, x_ovf, y_ovf;
			short y_delta, x_delta;
			y_delta = 0; x_delta = 0;

			if((packet[0] & Y_OVF) == 0)
				y_ovf = 0;
			else
				y_ovf = 1;

			if((packet[0] & X_OVF) == 0)
				x_ovf = 0;
			else
				x_ovf = 1;

			if((packet[0] & Y_SIGN) != 0)
			{
				y_delta = (ONE_BYTE & ~(ONE_BYTE & packet[2]));
				y_delta += 1;
				y_delta = -y_delta;
			}
			else
				y_delta = packet[2];

			if((packet[0] & X_SIGN) != 0)
			{
				x_delta = (ONE_BYTE & ~(ONE_BYTE & packet[1]));
				x_delta += 1;
				x_delta = -x_delta;
			}
			else
				x_delta = packet[1];

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
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}


	tickdelay(micros_to_ticks(DELAY_US));

	sys_inb(KBD_OUT_BUF, &stat);

}



int test_async(unsigned short idle_time)
{
	mouse_hook = MOUSE_IRQ;
	tmr_hook = TIMER0_IRQ;

	if(kbd_mouse_subscribe_int() < 0)
		return -1;
	if(timer_subscribe(&tmr_hook) < 0)
		return -1;

	timed_mouse(idle_time);

	if(kbd_mouse_unsubscribe_int() < 0)
		return -1;
	if(timer_unsubscribe(&tmr_hook) < 0)
		return -1;

	return 0;
}

int timer_handler_mouse()
{
	counter = counter - 1;

	if(counter == 0)
	{
		return 1;
	}

	return 0;
}

int timed_mouse(unsigned short sec)
{
	int ipc_status;
	message msg;

	unsigned long irq_set_mouse = BIT(MOUSE_IRQ);
	unsigned long irq_set_tmr = BIT(TIMER0_IRQ);

	counter = sec*60;

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
				if (msg.NOTIFY_ARG & irq_set_mouse)
				{
					mouse_handler();
					counter = sec*60;
				}
				else if (msg.NOTIFY_ARG & irq_set_tmr)
				{
					terminus = timer_handler_mouse();
					if(terminus == 1)
						printf("Mouse scan ended because %u seconds have passed\n\n", sec);
				}
				break;
			default:
				break;
			}
		}
	}

	return 0;
}



int test_config(void)
{
	//dis_stream();
	kbd_mouse_subscribe_int();
    read_config();
    kbd_mouse_unsubscribe_int();
    printf("\tDone\n");
    return 0;
}

void dis_stream()
{
	unsigned long stat;

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
			sys_outb(KBD_IN_BUF, DIS_STREAM_MODE);
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}

	tickdelay(micros_to_ticks(DELAY_US));

	sys_inb(KBD_OUT_BUF, &stat);
}

void read_config()
{

	unsigned long stat;

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
			sys_outb(KBD_IN_BUF, DIS_STREAM_MODE);
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}

	tickdelay(micros_to_ticks(DELAY_US));

	sys_inb(KBD_OUT_BUF, &stat);

	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////

	unsigned char bytes[3];

	while(1)
	{
		unsigned long stat;

		sys_outb(KBC_CMD_REG, WRITE_MOUSE);

		tickdelay(micros_to_ticks(DELAY_US));

		sys_inb(STAT_REG, &stat);
		//printf("FIRST : 0x%x", stat);

		sys_outb(KBD_IN_BUF, STATUS_REQ);

		//tickdelay(micros_to_ticks(DELAY_US));

		sys_inb(STAT_REG, &stat);
		//printf("SECOND : 0x%x", stat);

		sys_inb(KBD_OUT_BUF, &stat);
		//printf("Third : 0x%x", stat);

		//if(stat != ACK)
		//	continue;

		sys_inb(KBD_OUT_BUF, &stat);
		//tickdelay(micros_to_ticks(DELAY_US));
		bytes[0] = stat;
		sys_inb(KBD_OUT_BUF, &stat);
		//tickdelay(micros_to_ticks(DELAY_US));
		bytes[1] = stat;
		sys_inb(KBD_OUT_BUF, &stat);
		//tickdelay(micros_to_ticks(DELAY_US));
		bytes[2] = stat;

		print_config(bytes);

		return;
	}


	/*unsigned long stat;

	sys_outb(KBC_CMD_REG, WRITE_MOUSE);
	tickdelay(micros_to_ticks(DELAY_US));

	sys_outb(KBD_IN_BUF, DIS_STREAM_MODE);
	tickdelay(micros_to_ticks(DELAY_US));

	sys_inb(KBD_OUT_BUF, &stat);
	printf(" DISABLE RESPONSE : 0x%X\n", stat);

	sys_outb(0x64, WRITE_MOUSE);
	tickdelay(micros_to_ticks(DELAY_US));
	tickdelay(micros_to_ticks(DELAY_US));
	tickdelay(micros_to_ticks(DELAY_US));


	sys_inb(0x60, &stat);
	if((stat & IBF) != 0)
		printf("IBF FULL\n");

	sys_outb(0x60, STATUS_REQ);
	tickdelay(micros_to_ticks(DELAY_US));


	sys_inb(0x60, &stat);
	if((stat & IBF) != 0)
		printf("IBF FULL\n");

	sys_inb(0x60, &stat);
	printf(" STATUS REQUEST 1 : 0x%X\n", stat);
	tickdelay(micros_to_ticks(DELAY_US));

	sys_inb(0x60, &stat);
	if((stat & IBF) != 0)
		printf("IBF FULL\n");

	sys_inb(KBD_OUT_BUF, &stat);
	printf(" STATUS REQUEST 2 : 0x%X\n", stat);
	tickdelay(micros_to_ticks(DELAY_US));

	sys_inb(KBD_OUT_BUF, &stat);
	printf(" STATUS REQUEST 3 : 0x%X\n", stat);
	tickdelay(micros_to_ticks(DELAY_US));

	sys_inb(KBD_OUT_BUF, &stat);
	printf(" STATUS REQUEST 4 : 0x%X\n", stat);
	tickdelay(micros_to_ticks(DELAY_US));*/
}

void print_config(unsigned char status[])
{
	printf("===> Mouse configuration : \n\n");

	printf("Byte 1 : 0x%02X\n", status[0]);
	printf("Byte 2 : 0x%02X\n", status[1]);
	printf("Byte 3 : 0x%02X\n", status[2]);


	if((REMOTE & status[0]) == 0)
	{
		printf("Stream mode.\n");

		if((ENABLE & status[0]) == 0)
			printf("Data Reporting disabled.\n");
		else
			printf("Data Reporting enabled.\n");
	}
	else
		printf("Remote mode.\n");

	if ((SCALING & status[0])==0)
		printf("Scaling is 1:1.\n");
	else
		printf("Scaling is 2:1.\n");

	if ((MB & status[0])==0)
		printf("Middle button released.\n");
	else
		printf("Middle button pressed.\n");

	if ((LB & status[0])==0)
		printf("Left button released.\n");
	else
		printf("Left button pressed.\n");

	if ((RB & status[0])==0)
		printf("Right button released.\n");
	else
		printf("Right button pressed.\n");

	int res = status[1] & RESOLUTION;

	switch(res)
	{
	case 0:
		printf("Resolution : 1 count/mm\n");
		break;
	case 1:
		printf("Resolution : 2 count/mm\n");
		break;
	case 2:
		printf("Resolution : 4 count/mm\n");
		break;
	case 3:
		printf("Resolution : 8 count/mm\n");
		break;
	}

	int sample_rate = status[2] & SAMPLE_RATE;

	printf("Sample rate : %u samples/second\n\n", sample_rate);
}


int mouse_read_packet(Mouse_packet *mouse)
{
	static unsigned char packet[3];

	unsigned long byte;
	byte = 0;
	sys_inb(KBD_OUT_BUF, &byte);
	static int packet_counter;

	//empty_buf();

	// Coding for packet_counter
	// 5 - first byte of first invalid packet already read
	// 6 - second byte of first invalid packet read. Since third is read this time, next is valid
	// 1 - must read first packet byte
	// 2 - must read second packet byte
	// 3 - must read third packet byte


	// This segment is used to prevent first couple of "bad" packets from
	// incorrectly move the mouse cursor

	if(packet_counter == 5)
	{
		packet_counter = 6;
		return 0;
	}

	if(packet_counter == 6)
	{
		packet_counter = 1;
		return 0;
	}

	if(packet_counter !=2 && packet_counter != 3 && packet_counter != 1)
	{
		packet_counter = 5;
		return 0;
	}

	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////

	if(packet_counter == 1)
	{
		packet[0] = 0;
		packet[1] = 0;
		packet[2] = 0;

		packet[0] = byte;


		if((packet[0] & PACKET_BYTE_1) == 0)
		{
			packet[0]=0;
			packet_counter = 1;
			return 0;
		}

		packet_counter = 2;
		return 0;
	}

	if(packet_counter == 2)
	{
		packet[1] = byte;
		packet_counter = 3;
		return 0;
	}

	if(packet_counter == 3)
	{
		packet[2] = byte;
		packet_counter = 1;

		if((packet[0] & Y_OVF) == 0)
			mouse->y_ovf = 0;
		else
			mouse->y_ovf = 1;

		if((packet[0] & X_OVF) == 0)
			mouse->x_ovf = 0;
		else
			mouse->x_ovf = 1;

		if((packet[0] & Y_SIGN) != 0)
		{
			mouse->y_delta = (ONE_BYTE & ~(ONE_BYTE & packet[2]));
			mouse->y_delta += 1;
			mouse->y_delta = - mouse->y_delta;
		}
		else
			mouse->y_delta = packet[2];

		if((packet[0] & X_SIGN) != 0)
		{
			mouse->x_delta = (ONE_BYTE & ~(ONE_BYTE & packet[1]));
			mouse->x_delta += 1;
			mouse->x_delta = - mouse->x_delta;
		}
		else
			mouse->x_delta = packet[1];

		if((packet[0] & MB) == 0)
			mouse->mb = 0;
		else
			mouse->mb = 1;

		if((packet[0] & LB) == 0)
			mouse->lb = 0;
		else
			mouse->lb = 1;

		if((packet[0] & RB) == 0)
			mouse->rb = 0;
		else
			mouse->rb = 1;


//		printf("B1=0x%X	B2=0x%X	B3=0x%X	LB=%u	MB=%u	RB=%u	XOV=%u	YOV=%u	X=%d	Y=%d\n", packet[0], packet[1],
//				packet[2], mouse->lb, mouse->mb, mouse->rb, mouse->x_ovf, mouse->y_ovf, mouse->x_delta, mouse->y_delta);

		return 1;
	}

	return 0;
}

void update_mouse(Mouse_coord *mouse, Mouse_packet *delta)
{
	mouse->x_coord += delta->x_delta;
	if(mouse->x_coord < 0)
		mouse->x_coord = 0;
	if(mouse->x_coord > 1024)
		mouse->x_coord = 1024;

	mouse->y_coord -= delta->y_delta;
	if(mouse->y_coord < 0)
		mouse->y_coord = 0;
	if(mouse->y_coord > 768)
		mouse->y_coord = 768;

	mouse->mb = delta->mb;
	mouse->lb = delta->lb;
	mouse->rb = delta->rb;

	mouse->img.x = mouse->x_coord;
	mouse->img.y = mouse->y_coord;

	if((delta->lb == 1) && ((int)mouse->img.map != (int)cursor_click) && (delta->rb != 1))
	{
		mouse->img.map = (char *)read_xpm(cursor_click, &mouse->img.width, &mouse->img.height);
	}
	else if((delta->lb == 0) && ((int)mouse->img.map != (int)cursor) && (delta->rb != 1))
	{
		mouse->img.map = (char *)read_xpm(cursor, &mouse->img.width, &mouse->img.height);
	}
	else if((delta->rb == 1) && (delta->lb == 1))
	{
		mouse->img.map = (char *)read_xpm(cursor_ghost_click, &mouse->img.width, &mouse->img.height);
	}
	else if((delta->rb == 1) && ((int)mouse->img.map != (int)cursor_ghost))
	{
		mouse->img.map = (char *)read_xpm(cursor_ghost, &mouse->img.width, &mouse->img.height);
	}
}
