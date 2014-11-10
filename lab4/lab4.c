#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <stdio.h>

#include "test4.h"
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
		  "    service run %s -args \"test_packet <int packet>\" \n"
		  "    service run %s -args \"test_async <int time-idle>\"\n"
		  "    service run %s -args \"test_config\"\n"
		  "    service run %s -args \"test_gesture <short length, unsigned short tolerance>\" \n", argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {

  /* check the function to test: if the first characters match, accept it */
  if (strncmp(argv[1], "test_packet", strlen("test_packet")) == 0) {
	  // check number of arguments
	  if( argc != 3 ) {
		  printf("mouse: wrong no of arguments for test of test_packet() \n");
		  return 1;
	  }

	  unsigned short packets = parse_ulong(argv[2], 10);

	  printf("mouse :: test_packet(%d)\n\n", packets);

	  test_packet(packets);

	  return 0;
  }
  else if (strncmp(argv[1], "test_async", strlen("test_async")) == 0)
  {
	  // check number of arguments
	  if( argc != 3 ) {
		  printf("mouse: wrong no of arguments for test of test_async() \n");
		  return 1;
	  }

	  unsigned short time = parse_ulong(argv[2], 10);

	  printf("mouse :: test_async(%d)\n\n", time);

	  test_async(time);

	  return 0;
  }
  else if (strncmp(argv[1], "test_config", strlen("test_config")) == 0)
  {
	  // check number of arguments
	  if( argc != 2 ) {
		  printf("mouse: wrong no of arguments for test of test_config() \n");
		  return 1;
	  }

	  printf("mouse :: test_config()\n\n");

	  test_config();

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
