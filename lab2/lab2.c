#include "timer.h"
#include "i8254.h"
#include <minix/drivers.h>

#include <stdio.h>

// Adapted lab1 functions
static void print_usage(char *argv[]);
static int proc_args(int argc, char *argv[]);
static unsigned long parse_char(char *str, int base);

int main(int argc, char **argv)
{
	sef_startup();

	// Uncomment for previous function
	/*printf("\n   ===== Test functions start here ===== \n \n");
	printf(" ==> Testing timer_test_config(unsigned char timer)\n\n");

	// Timer control register setup - Control:0x16
	char tmr_conf = 0;
	tmr_conf |= TIMER_SEL0;
	tmr_conf |= TIMER_LSB;
	tmr_conf |= TIMER_SQR_WAVE;
	sys_outb(TIMER_CTRL, tmr_conf);
	sys_outb(TIMER_0, 0);
	printf("Timer control register: 0x16 - 00010110b\n\n");
	//////////////////////////////////////////////

	timer_test_config(0);*/

	// Using test configs from lab1

	if ( argc == 1 ) {
	      print_usage(argv);
	      return 0;
	  } else {
	      proc_args(argc, argv);
	  }

	return 0;
}

static void print_usage(char *argv[]) {
  printf("\n\nUsage: one of the following:\n"
	 "\t service run %s -args \"test_config <int-timer>\" \n", argv[0]);
}

static int proc_args(int argc, char *argv[]) {

	unsigned int timer = 0;
	unsigned long cmd;

  /* check the function to test: if the first characters match, accept it */
  if (strncmp(argv[1], "test_config", strlen("test_config")) == 0) {
	  if( argc != 3 ) {
		  printf("timer: wrong no of arguments for test of timer_test_config() \n");
		  return 1;
	  }

	  if(strncmp(argv[2], "0", 1) == 0)
		  timer = 0;
	  else if (strncmp(argv[2], "1", 1) == 0)
		  timer = 1;
	  else if (strncmp(argv[2], "2", 1) == 0)
		  timer = 2;
	  else
	  {
		  printf("timer: invalid timer indicated - must be in range 0 - 2 \n");
		  return 1;
	  }

	  printf("timer :: timer_test_config(%d)\n\n", timer);

	  timer_test_config(timer);

	  return 0;
  }
}

static unsigned long parse_char(char *str, int base) {
  char *endptr;
  unsigned long val;

  val = strtoul(str, &endptr, base);

  if ((errno == ERANGE && val == ULONG_MAX )
	  || (errno != 0 && val == 0)) {
	  perror("strtol");
	  return ULONG_MAX;
  }

  if (endptr == str) {
	  printf("video_txt: parse_ulong: no digits were found in %s \n", str);
	  return ULONG_MAX;
  }

  /* Successful conversion */
  return val;
}
