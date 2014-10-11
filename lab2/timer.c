#include <minix/syslib.h>
#include <minix/drivers.h>

// Our "includes" /////////////////////////////////
#include "i8254.h"
#include <stdio.h>

//TO-DO
int timer_set_square(unsigned long timer, unsigned long freq) {

	return 1;
}

//TO-DO
int timer_subscribe_int(void ) {

	return 1;
}

//TO-DO
int timer_unsubscribe_int() {

	return 1;
}

//TO-DO
void timer_int_handler() {

}

// Completed
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
			sys_inb(TIMER_0, st);
			break;
		case 1:
			sys_inb(TIMER_1, st);
			break;
		case 2:
			sys_inb(TIMER_2, st);
			break;
		}


	return 0;
}

// Completed
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

// TO-DO
int timer_test_square(unsigned long freq) {
	
	return 1;
}

// TO-DO
int timer_test_int(unsigned long time) {
	
	return 1;
}

// Completed
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
