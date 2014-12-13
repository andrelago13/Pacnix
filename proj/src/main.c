#include "pacnix.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	time_t t;
	srand((unsigned) time(&t));
	sef_startup();

	pacnix_start();

	return 0;
}
