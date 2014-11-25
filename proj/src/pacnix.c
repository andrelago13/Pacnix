#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <stdio.h>
#include <minix/sysutil.h>
#include <stdint.h>

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
}

void interrupts()
{
	int ipc_status;
	message msg;


	int mouse_hook = MOUSE_IRQ;
	int ret = mouse_subscribe(&mouse_hook);
	if(ret < 0)
		return;
	unsigned long irq_set_mouse = BIT(ret);

	int timer_hook = TIMER0_IRQ;
	ret = timer_subscribe(&timer_hook);
	if(ret < 0)
		return;
	unsigned long irq_set_timer = BIT(ret);

	ret = kbd_subscribe_int();
	if(ret < 0)
		return;
	unsigned long irq_set_kbd = BIT(ret);

	int terminus = 1;

	Mouse_coord mouse, to_delete;
	mouse.x_coord = 450;
	mouse.y_coord = 400;
	mouse.img.x = mouse.x_coord;
	mouse.img.y = mouse.y_coord;
	mouse.img.map = (char *)read_xpm(cursor, &mouse.img.width, &mouse.img.height);

	to_delete = mouse;

	Mouse_packet tmp_delta;
	tmp_delta.x_delta = 0;
	tmp_delta.y_delta = 0;
	tmp_delta.x_ovf = 0;
	tmp_delta.y_ovf = 0;
	tmp_delta.rb = 0;
	tmp_delta.lb = 0;
	tmp_delta.mb = 0;

	set_stream();
	timer_set_square(0, 51);
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
				if (msg.NOTIFY_ARG & irq_set_mouse)
				{
					ret = mouse_read_packet(&tmp_delta);
					if(ret == 1)
					{
						update_mouse(&mouse, &tmp_delta);
						if((tmp_delta.lb == 1) && (tmp_delta.rb == 1))
							terminus = 0;
					}
				}
				else if (msg.NOTIFY_ARG & irq_set_timer)
				{
					erase_mouse(&to_delete);
					draw_mouse(&mouse);
					to_delete = mouse;
				}
				else if (msg.NOTIFY_ARG & irq_set_kbd)
				{
					unsigned long letra = 0;

					if(OK != sys_inb(KBD_OUT_BUF, &letra))		// Read scancode
						return;

					if(letra == ESC_break)
						terminus = 0;
				}
				break;
			default:
				break;
			}
		}
	}
	unsigned long byte;
	printf("Terminus 1\n");
	sys_inb(KBD_OUT_BUF, &byte);
	tickdelay(micros_to_ticks(DELAY_US));
	printf("Terminus 2\n");
	sys_inb(KBD_OUT_BUF, &byte);
	printf("\n\tDone\n");
	mouse_unsubscribe(&mouse_hook);
	printf("Mouse unsubscribed\n");
	timer_unsubscribe(&timer_hook);
	printf("Timer unsubscribed\n");
	kbd_unsubscribe_int();
	printf("Keyboard unsubscribed\n");

	vg_exit();
	printf("Text mode back\n");
}
