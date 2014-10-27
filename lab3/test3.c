#include "test3.h"
#include "kbd.h"
#include "timer.h"
#include "i8254.h"
//#include "asmhandler.S"

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <stdio.h>
#include <minix/sysutil.h>

int kbd_hook, tmr_hook, counter;

// Assembly function declaration
unsigned long asmHandler(unsigned long letra);


// Test_Scan  /////////////////////////////////////////////////////////////////////

int kbd_test_scan(unsigned short ass)
{
	if(ass == 0)
		return kbd_interrupts(kbd_c_handler);		//Choose to process C code
	else
		return kbd_interrupts(kbd_asm_handler);		//Choose to process assembly code

	return 1;
}

int kbd_interrupts(int handler())
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
					terminus = handler();
				}
				break;
			default:
				break;
			}
		}
	}

	if(kbd_unsubscribe_int() != 0)
		return 1;

	printf("Scan ended due to ESC breakcode\n\n");

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

int kbd_c_handler()
{
	unsigned long letra = 0;

	static unsigned int prev_spec;

	if(OK != sys_inb(KBD_OUT_BUF, &letra))		// Read scancode
		return 1;

	if(letra == TWO_BYTE_SCAN)					// Check if scancode starts with 0xE0, i.e., has 2 bytes
	{
		printf("Special key => ");
		prev_spec = 1;
		return 0;
	}

	if((letra & BREAK_CODE) != 0)			// Check if scancode is break or make code
		if(prev_spec == 1)
		{
			printf("Breakcode : 0xE0%02X\n", letra);
			prev_spec = 0;
		}
		else
			printf("Breakcode : 0x%02X\n", letra);
	else
		if(prev_spec == 1)
		{
			printf("Makecode : 0xE0%02X\n", letra);
			prev_spec = 0;
		}
		else
			printf("Makecode : 0x%02X\n", letra);



	if(letra == ESC_break)
		return 1;

	return 0;
}

int kbd_asm_handler()
{
	static unsigned long asm_letra;

	sys_enable_iop(SELF);	// wrote this since sys_iopenable was not recognized

	asm_letra = asmHandler(asm_letra);

	if(asm_letra == ESC_break)
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

		if(status < 0)
			return 1;

		switch(leds[i])				//Checks toogle_led array and according to it turns ON/OFF
		{							//the scroll lock/Caps lock/Numeric lock
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
																						///////////////////////////////////////////////////////////////
		wait_x_sec(1);																	////////////////////// FALTA ACABAR ///////////////////////////
																						///////////////////////////////////////////////////////////////
		i++;
	}

	printf("\nDone\n\n");

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

	int endAll = 0;				// Used to stop master cycle
	int end1 = 0;				// Used to stop slave cycle
	unsigned long ret;			// Used to analyse KBC return

	while(endAll == 0)
	{

		while(end1 == 0)		// This cycle is repeated until command is written successfully
		{
			if(OK != sys_outb(KBD_IN_BUF, SET_RESET_CMD))
				return 1;

			tickdelay(micros_to_ticks(DELAY_US));

			if(OK != sys_inb(KBD_OUT_BUF, &ret))
				return 1;

			if(ret == ACK)
				end1 = 1;
		}

		if(OK != sys_outb(KBD_IN_BUF, led_cmd))
			return 1;

		tickdelay(micros_to_ticks(DELAY_US));

		if(OK != sys_inb(KBD_OUT_BUF, &ret))
			return 1;

		switch(ret)
		{
		case ACK:
			endAll = 1;
			break;
		case RESEND:				// Must resend last byte, continue works because end1 == 1
			continue;
			break;
		case ERR:					// Must repeat from the beginnig. For that, end1 must be reset
			end1 = 0;
			continue;
			break;
		}

	}

	return status;
}


// Test_timed_scan  /////////////////////////////////////////////////////////////////////

int kbd_test_timed_scan(unsigned short n)
{
    kbd_hook = KBD_IRQ;
    tmr_hook = TIMER0_IRQ;

    kbd_subscribe_int();
    timer_subscribe(&tmr_hook);

    timed_scan(n);

    kbd_unsubscribe_int();
    timer_unsubscribe(&tmr_hook);
}

int timer_handler()
{
	counter = counter - 1;

	if(counter == 0)
	{
		return 1;
	}

	return 0;
}

int timed_scan(unsigned short time)
{
	int ipc_status;
	message msg;

	unsigned long irq_set_kbd = BIT(KBD_IRQ);
	unsigned long irq_set_tmr = BIT(TIMER0_IRQ);

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
				if (msg.NOTIFY_ARG & irq_set_kbd)
				{
					terminus = kbd_c_handler();
					counter = time * 60;
					if(terminus == 1)
						printf("Scan ended due to ESC breakcode\n\n");
				}
				else if (msg.NOTIFY_ARG & irq_set_tmr)
				{
					terminus = timer_handler();
					if(terminus == 1)
						printf("Scan ended because %u seconds have passed\n\n", time);
				}
				break;
			default:
				break;
			}
		}
	}

	return 0;
}
