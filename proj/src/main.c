#include "pacnix.h"
#include "video.h"
#include <stdio.h>
#include <stdlib.h>

#include "pac_menu.h"

int main()
{
	time_t t;
	srand((unsigned) time(&t));
	sef_startup();

	vg_init(GRAF_1024x768);	//
	vg_exit();				// to prevent "darker colors" on first run
	vg_init(GRAF_1024x768);
	pacnix_start();
	vg_exit();
	printf("\n\n\tProgram ended\n\n\tThanks for playing!\n\n");

//	FILE * fp;
//	fp = (FILE *)open("scores.txt", "r");
//	char *str1 = "hello";
//
//	int c;
//	c = fgetc(fp);
//
//	//fprintf(fp, "%s %s %s %d", "We", "are", "in", 2012);
//	//fprintf(fp,  "more: %s\n", str1);
//	fclose(fp);

	printf("HI\n");

	return 0;




}
