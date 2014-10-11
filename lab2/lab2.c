#include "timer.h"
#include "i8254.h"
#include <minix/drivers.h>

#include <stdio.h>

// Adapted lab1 functions
static void print_usage(char *argv[]);
static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);

int main(int argc, char **argv)
{
	sef_startup();

	// Using test configurations from lab1

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
	 "\t service run %s -args \"test_config <int-timer>\" \n\t service run %s -args \"test_square <ulong freq>\" \n"
	 "\t service run %s -args \"test_int <int-time>\" \n", argv[0], argv[0], argv[0]);
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

	  // Find out the selected timer
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
  else if (strncmp(argv[1], "test_square", strlen("test_square")) == 0)
  {
	  if( argc != 3 ) {
		  printf("timer: wrong no of arguments for test of timer_test_square() \n");
		  return 1;
	  }

	  unsigned long freq = parse_ulong(argv[2], 10);

	  printf("timer :: timer_test_square(%ul)\n\n", freq);

	  timer_test_square(freq);

	  return 0;
  }
  else if (strncmp(argv[1], "test_int", strlen("test_int")) == 0)
    {
  	  if( argc != 3 ) {
  		  printf("timer: wrong no of arguments for test of timer_test_int() \n");
  		  return 1;
  	  }

  	  unsigned long time = parse_ulong(argv[2], 10);

  	  printf("timer :: timer_test_int(%ul)\n\n", time);

  	  timer_test_int(time);;

  	  return 0;
    }
}

// Function provided in Lab1 to convert text argument to numerical value
static unsigned long parse_ulong(char *str, int base) {
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

