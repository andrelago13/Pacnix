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



// Test_Scan  /////////////////////////////////////////////////////////////////////

int kbd_test_scan(unsigned short ass)
{
	if(ass == 0)
		return kbd_interrupts();
	else
	{
		/*   TO-DO   */
	}

	return 0;
}

int kbd_interrupts()
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
					terminus = kbd_int_handler();
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

	return ret;
}

int kbd_int_handler()
{
	unsigned long letra = 0;

	static unsigned int prev_spec;

	sys_inb(KBD_OUT_BUF, &letra);

	if(letra == 0xE0)
	{
		printf("Special key => ");
		prev_spec = 1;
		return 0;
	}

	if((letra & BIT(7)) != 0)
		if(prev_spec == 1)
		{
			printf("Breakcode : 0xE0%x\n", letra);
			prev_spec = 0;
		}
		else
			printf("Breakcode : 0x%x\n", letra);
	else
		if(prev_spec == 1)
		{
			printf("Makecode : 0xE0%x\n", letra);
			prev_spec = 0;
		}
		else
			printf("Makecode : 0x%x\n", letra);



	if(letra == 0x81)
		return 1;

	return 0;
}



// Test_Leds  /////////////////////////////////////////////////////////////////////

int kbd_test_leds(unsigned short n, unsigned short *leds)
{
	unsigned int i = 0;

	while(i < n)
	{
		int status = toggle_led(leds[i]);

		switch(leds[i])
		{
		case 0:
			if(status == 1)
				printf("Switched scroll lock ON\n");
			else
				printf("Switched scroll lock OFF\n");
			break;
		case 1:
			if(status == 1)
				printf("Switched numeric lock ON\n");
			else
				printf("Switched numeric lock OFF\n");
			break;
		case 2:
			if(status == 1)
				printf("Switched caps lock ON\n");
			else
				printf("Switched caps lock OFF\n");
			break;
		}

		wait_x_sec(1);

		i++;
	}

	return 0;
}

int toggle_led(unsigned short led)
{
	int status = 0;

	static unsigned int caps, num, scroll;

	if(caps != 1)
		caps = 0;
	if(num != 1)
		num = 0;
	if(scroll != 1)
		scroll = 0;

	switch(led)
	{
	case 0:
		scroll = scroll ^ 1;
		status = scroll;
		break;
	case 1:
		num = num ^ 1;
		status = num;
		break;
	case 2:
		caps = caps ^ 1;
		status = caps;
		break;
	}

	unsigned int temp1 = caps << 2;
	unsigned int temp2 = num << 1;

	unsigned long led_cmd = 0;
	led_cmd |= scroll;
	led_cmd |= temp2;
	led_cmd |= temp1;

	sys_outb(KBD_OUT_BUF, SET_RESET_CMD);
	tickdelay(micros_to_ticks(DELAY_US));
	sys_outb(KBD_OUT_BUF, led_cmd);

	return status;
}


// Test_timed_scan  /////////////////////////////////////////////////////////////////////

int kbd_test_timed_scan(unsigned short n)
{
    /* To be completed */
}
