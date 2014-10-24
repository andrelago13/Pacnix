#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>

// Our "includes" ///////////////////////////////////
#include "i8254.h"
#include <stdio.h>

unsigned int t0_hook;
unsigned int counter = 0;
unsigned int msg_counter = 0;

// Programs 'timer' to perform interrupts on frequency 'freq'
int timer_set_square(unsigned long timer, unsigned long freq)
{
	if(freq > TIMER_FREQ || freq <= 0)
		return 1;

	unsigned long *ctrl;		// ctrl is the control register word, used to preserve final 4 bits
	ctrl = malloc(sizeof(unsigned long));
	sys_inb(TIMER_CTRL, ctrl);

	// bit mask to preserve least significant 4 bits and erase the rest
	unsigned char mask = BIT(0) | BIT(1) | BIT(2) | BIT(3);
	*ctrl = *ctrl & mask;

	// selecting type of access
	*ctrl = *ctrl & TIMER_LSB_MSB;

	// selecting timer
	switch (timer)
	{
	case 0:
		*ctrl = *ctrl & TIMER_SEL0;
		break;
	case 1:
		*ctrl = *ctrl & TIMER_SEL1;
		break;
	case 2:
		*ctrl = *ctrl & TIMER_SEL2;
		break;
	default:
		return 1;
	}

	unsigned long clock_value;		// initial timer count

	clock_value = TIMER_FREQ/freq;

	if(clock_value >= BIT(16))
		return 1;

	unsigned long timer_lsb, timer_msb;

	mask = 0; mask = ~mask;
	timer_lsb = mask & clock_value;
	timer_msb =  clock_value >> 8;

	int ret = 0;

	ret = sys_outb(TIMER_CTRL, *ctrl); // Writing control word

	if(ret != 0)
		return 1;

	// Writing initial counter on timer
	switch (timer)
	{
	case 0:
		sys_outb(TIMER_0, timer_lsb);
		if(ret != 0)
			return 1;
		sys_outb(TIMER_0, timer_msb);
		break;
	case 1:
		sys_outb(TIMER_1, timer_lsb);
		if(ret != 0)
			return 1;
		sys_outb(TIMER_1, timer_msb);
		break;
	case 2:
		sys_outb(TIMER_2, timer_lsb);
		if(ret != 0)
			return 1;
		sys_outb(TIMER_2, timer_msb);
		break;
	}

	if(ret != 0)
		return 1;

	return 0;
}

// Subscribes timer 0 interrupts with t0_hook hook_id
int timer_subscribe_int(void )
{
	t0_hook = TIMER0_IRQ;

	int ret = sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &t0_hook);

	if(ret < 0)
		return -1;

	return TIMER0_IRQ;
}

// Unsubscribes timer 0 interrupts with t0_hook hook_id
int timer_unsubscribe_int()
{
	int ret = sys_irqrmpolicy(&t0_hook);

	return ret;
}

// Prints a message if 1 second (60 interrupts) has passed and if counter > 0
void timer_int_handler()
{

	if (counter > 0 && (msg_counter % 60 == 0))
	{
		printf("One more second\n");
		counter--;
	}

	if (counter == 0)
	{
		timer_unsubscribe_int();
		msg_counter++;
		return;
	}

	msg_counter++;
}

// Checks for timer 0 interrupts and calls int_handler if occurs. Also checks the counter
void t0_int()
{
	int ipc_status;
	message msg;
	unsigned long irq_set;

	irq_set = BIT(timer_subscribe_int());
	int terminus = 0;

	while(terminus == 0)
	{

		if(driver_receive(ANY, &msg, &ipc_status)!=0)
		{
			printf("Driver_receive failed with");
			continue;
		}

		if(is_ipc_notify(ipc_status))
		{
			switch(_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE:
				if(msg.NOTIFY_ARG & irq_set)
				{
					timer_int_handler();
					if (counter == 0)
					{
						terminus = 1;
					}
				}
				break;
			default:
				break;
			}
		}
	}
}

// Saves the 'timer' configuration on '*st'
int timer_get_conf(unsigned long timer, unsigned char *st) {
	
	char command = 0;		// variable for read-back command selection
	command = command | TIMER_RB_CMD;

	switch(timer)
	{
	case 0:
		command = command | TIMER_RB_SEL(0);
		break;
	case 1:
		command = command | TIMER_RB_SEL(1);
		break;
	case 2:
		command = command | TIMER_RB_SEL(2);
		break;
	default :
		return 1;
	}

	// int sys_inb(port_t port, unsigned long *byte);
	// int sys_outb(port_t port, unsigned long byte);

	sys_outb(TIMER_CTRL, command);

	switch(timer)
		{
		case 0:
			sys_inb(TIMER_0, (long unsigned int *) st);
			break;
		case 1:
			sys_inb(TIMER_1, (long unsigned int *) st);
			break;
		case 2:
			sys_inb(TIMER_2, (long unsigned int *) st);
			break;
		}


	return 0;
}

// Displays a timer's configuration, saved as unsigned char in 'conf'
int timer_display_conf(unsigned char conf) {
	
	unsigned char mask = BIT(7);

	if((conf & mask) == 0)
		printf("Timer output: 0\n");
	else
		printf("Timer output: 1\n");

	mask = BIT(6);

	if((conf & mask) == 0)
		printf("Null count: 0\n");
	else
		printf("Null count: 1\n");

	mask = BIT(5);  // 00100000b

	if((conf & mask) == 0)
		printf("Type of access: LSB\n");
	else
	{
		mask = BIT(4);
		if((conf & mask) == 0)
			printf("Type of access: MSB\n");
		else
			printf("Type of access: LSB followed by MSB\n");
	}

	mask = BIT(3)|BIT(2)|BIT(1);
	unsigned char temp;
	temp = conf & mask;

	switch(temp)
	{
	case 0:
		printf("Programmed mode: 0\n");
		break;
	case BIT(1):
		printf("Programmed mode: 1\n");
		break;
	case BIT(2):
		printf("Programmed mode: 2\n");
		break;
	case BIT(3)|BIT(2):
		printf("Programmed mode: 2\n");
		break;
	case BIT(2)|BIT(1):
		printf("Programmed mode: 3\n");
		break;
	case BIT(3)|BIT(2)|BIT(1):
		printf("Programmed mode: 3\n");
		break;
	case BIT(3):
		printf("Programmed mode: 4\n");
		break;
	case BIT(3)|BIT(1):
		printf("Programmed mode: 5\n");
		break;
	}

	mask = TIMER_BCD;

	if((conf & mask) == 0)
		printf("Counting mode : Binary\n");
	else
		printf("Counting mode : BCD\n");



	return 0;
}

// Programs timer 0 to operate on the given frequency 'freq', and displays timer configuration next
int timer_test_square(unsigned long freq)
{
	if(freq <= 0 || freq > TIMER_FREQ || TIMER_FREQ/freq >= BIT(16))
	{
		printf("ERROR : Invalid timer frequency.\n");
		return 1;
	}

	if(timer_set_square(0, freq) !=0)
		return 1;

	return timer_test_config(0);
}

// Prints a message every second (60 timer 0 interrupts) for 'time' seconds
int timer_test_int(unsigned long time) {
	
	counter = time;


	t0_int();

	return 0;
}

// Gets and displays the configuration of a timer
int timer_test_config(unsigned long timer) {
	
	unsigned char *st;
	st = malloc(sizeof(unsigned char));

	if(timer_get_conf(timer, st) != 0)
		{
			free(st);
			return 1;
		}

	timer_display_conf(*st);

	free(st);

	return 0;
}




void one_sec()
{
	msg_counter++;

	if (counter > 0 && (msg_counter % 60 == 0))
	{
		counter = 0;
		timer_unsubscribe_int();
		msg_counter = 0;
	}
}


void wait_x_sec(unsigned int x)
{
	counter = 1;

	int ipc_status;
	message msg;
	unsigned long irq_set;

	irq_set = BIT(timer_subscribe_int());
	int terminus = 0;

	while(terminus == 0)
	{

		if(driver_receive(ANY, &msg, &ipc_status)!=0)
		{
			printf("Driver_receive failed\n");
			continue;
		}

		if(is_ipc_notify(ipc_status))
		{
			switch(_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE:
				if(msg.NOTIFY_ARG & irq_set)
				{
					one_sec();
					if (counter == 0)
					{
						terminus = 1;
					}
				}
				break;
			default:
				break;
			}
		}
	}
}
