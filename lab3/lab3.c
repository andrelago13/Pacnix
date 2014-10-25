#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <stdio.h>

#include "test3.h"
#include "kbd.h"


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

// Prints the how-to work with the test functions
static void print_usage(char *argv[]) {
  printf("\n\nUsage: one of the following:\n"
	 "\t service run %s -args \"test_scan <0 - C, 1 - Assembly>\" \n"
	 "\t service run %s -args \"test_leds <int 0-2 sequence>\"\n"
	 "\t service run %s -args \"test_timed_scan <int-seconds>\" \n", argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {

	unsigned short assembly;

  /* check the function to test: if the first characters match, accept it */
  if (strncmp(argv[1], "test_scan", strlen("test_scan")) == 0) {
	  // check number of arguments
	  if( argc != 3 ) {
		  printf("keyboard: wrong no of arguments for test of kbd_test_scan() \n");
		  return 1;
	  }

	  // Find out the selected timer
	  if(strncmp(argv[2], "0", 1) == 0)
		  assembly = 0;
	  else if (strncmp(argv[2], "1", 1) == 0)
		  assembly = 1;
	  else
	  {
		  printf("keyboard: invalid value for assembly - must be 0 or 1\n");
		  return 1;
	  }

	  printf("keyboard :: kbd_test_scan(%d)\n\n", assembly);

	  kbd_test_scan(assembly);

	  return 0;
  }
  else if (strncmp(argv[1], "test_leds", strlen("test_leds")) == 0)
  {
	  if( argc < 3 ) {
		  printf("keyboard: wrong no of arguments for test of kbd_test_leds() \n");
		  return 1;
	  }

	  unsigned short *leds = malloc(sizeof(unsigned short)*(argc - 2));

	  unsigned int i = 0;
	  for(; i < argc-2; i++)
	  {
		  unsigned short val = parse_ulong(argv[i+2], 10);
		  if(val < 0 || val > 2)
		  {
			  printf("keyboard: invalid value for led - must be in range 0 - 2\n");
			  return 1;
		  }
		  leds[i] = val;
	  }

	  printf("keyboard :: kbd_test_leds(%u, [", argc-2);

	  for(i = 0; i < argc-2; i++)
	  {
		  printf(" %u ", leds[i]);
		  if(i < argc-3)
			  printf(",");
	  }
	  printf("]\n\n");

	  kbd_test_leds(argc-2, leds);

	  return 0;
  }
  else if (strncmp(argv[1], "test_timed_scan", strlen("test_timed_scan")) == 0)
    {
  	  if( argc != 3 ) {
  		  printf("keyboard: wrong no of arguments for test of kbd_test_timed_scan() \n");
  		  return 1;
  	  }

  	  unsigned short time = parse_ulong(argv[2], 10);

  	  printf("keyboard :: kbd_test_timed_scan(%u)\n\n", time);

  	  kbd_test_timed_scan(time);;

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
