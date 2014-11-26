#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <stdio.h>
#include <minix/sysutil.h>
#include <stdint.h>
#include <math.h>

#include "pacnix.h"
#include "mouse.h"
#include "mouse_header.h"
#include "timer.h"
#include "video_gr.h"
#include "i8254.h"
#include "kbd_header.h"
#include "pixmaps.h"
#include "video.h"
#include "kbd_header.h"
#include "kbd_funct.h"

void pacnix_start()
{
	interrupts();
	printf("\n\n\tProgram ended\n\n");
}

void interrupts()
{
	int ipc_status;
	message msg;

	// Subscribe mouse interrupts
	int mouse_hook = MOUSE_IRQ;
	int ret = mouse_subscribe(&mouse_hook);
	if(ret < 0)
		return;
	unsigned long irq_set_mouse = BIT(ret);

	// Subscribe timer0 interrupts
	int timer_hook = TIMER0_IRQ;
	ret = timer_subscribe(&timer_hook);
	if(ret < 0)
		return;
	unsigned long irq_set_timer = BIT(ret);

	// Subscribe keyboard interrupts
	ret = kbd_subscribe_int();
	if(ret < 0)
		return;
	unsigned long irq_set_kbd = BIT(ret);

	int terminus = 1;

	// Initialize mouse cursor
	Mouse_coord mouse;
	mouse.x_coord = 450;
	mouse.y_coord = 400;
	mouse.img.x = mouse.x_coord;
	mouse.img.y = mouse.y_coord;
	mouse.img.map = (char *)read_xpm(cursor, &mouse.img.width, &mouse.img.height);

	// Initialize packet_read
	Mouse_packet tmp_delta;

	// Initialize mouse stream mode
	set_stream();

	// Initialize frame rate counters. Frame rate set to 50

	double period_ints = (double) 50 / 60;
	double sum_period = 0;

	// Initialize VRAM
	vg_init(GRAF_1024x768);

	while(terminus != 0)
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
				if (msg.NOTIFY_ARG & irq_set_mouse)		//// MOUSE INTERRUPT ////
				{
					ret = mouse_read_packet(&tmp_delta);
					if(ret == 1)
					{
						if((tmp_delta.lb == 1) && ((int)mouse.img.map != (int)cursor_click) && (tmp_delta.rb != 1))
						{
							mouse.img.map = (char *)read_xpm(cursor_click, &mouse.img.width, &mouse.img.height);
						}
						else if((tmp_delta.lb == 0) && ((int)mouse.img.map != (int)cursor) && (tmp_delta.rb != 1))
						{
							mouse.img.map = (char *)read_xpm(cursor, &mouse.img.width, &mouse.img.height);
						}
						else if((tmp_delta.rb == 1) && (tmp_delta.lb == 1))
						{
							mouse.img.map = (char *)read_xpm(cursor_ghost_click, &mouse.img.width, &mouse.img.height);
						}
						else if((tmp_delta.rb == 1) && ((int)mouse.img.map != (int)cursor_ghost))
						{
							mouse.img.map = (char *)read_xpm(cursor_ghost, &mouse.img.width, &mouse.img.height);
						}

						update_mouse(&mouse, &tmp_delta);
					}
				}
				else if (msg.NOTIFY_ARG & irq_set_timer)		//// TIMER 0 INTERRUPT ////
				{
					sum_period += period_ints;

					if(abs(sum_period-1) < 0.1)
					{
						sum_period = 0;
						fill_screen(COLOR_BLACK);
						draw_mouse(&mouse);
						update_buffer();
					}
				}
				else if (msg.NOTIFY_ARG & irq_set_kbd)			//// KEYBOARD INTERRUPT ////
				{
					unsigned long letra = 0;

					if(OK != sys_inb(KBD_OUT_BUF, &letra))	// Read scancode
						return;

					if(letra == ESC_break)
					{
						terminus = 0;
						dis_stream();
					}
				}
				break;
			default:
				break;
			}
		}
	}

	// Unsubscribe all interrupts
	mouse_unsubscribe(&mouse_hook);
	timer_unsubscribe(&timer_hook);
	kbd_unsubscribe_int();

	vg_exit();
}
