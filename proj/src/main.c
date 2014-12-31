#include "pacnix.h"
#include "video.h"
#include <stdio.h>
#include <stdlib.h>

#include "pac_menu.h"
#include "mouse.h"
#include "video_gr.h"

/**
\mainpage PACNIX - LCOM project documentation

This is the documentation for PACNIX, a project made by students for the LCOM class
of the 2nd year of the "Mestrado Integrado em Engenharia Informática e Computação" of
the "Faculdade De Engenharia Da Universidade Do Porto".

The intent of the project is to be our own version of the classic arcade game, Pacman,
using most of the hardware of a common PC.
We respect the original creators of the game, and thank them for the inspiration for our work.

This documentation has been organized according to the modules of the project
*/

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

	return 0;




}
