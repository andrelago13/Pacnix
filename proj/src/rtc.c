#include "rtc.h"
#include <minix/drivers.h>


void rtc_dis_interrupts()
{
	asm("cli");
}

void rtc_ena_interrupts()
{
	asm("sti");
}

unsigned char rtc_bcd_to_bin(unsigned char bincode_dec)
{
	return ((bincode_dec >> 4) * 10) + (bincode_dec & 0x0F);
}

int rtc_get_time(unsigned long *hour, unsigned long *min, unsigned long *sec)
{
	rtc_dis_interrupts();

	unsigned long result;

	while(result & 0x80)
	{
		if (sys_outb(RTC_ADDR_REG, RTC_REG_A))
			return 1;
		if (sys_inb(RTC_DATA_REG, &result))
			return 1;
	}

	if (sys_outb(RTC_ADDR_REG, HOUR))
		return 1;
	if (sys_inb(RTC_DATA_REG, hour))
		return 1;

	while(result & 0x80)
	{
		if (sys_outb(RTC_ADDR_REG, RTC_REG_A))
			return 1;
		if (sys_inb(RTC_DATA_REG, &result))
			return 1;
	}

	if(sys_outb(RTC_ADDR_REG, MINUTE))
		return 1;
	if(sys_inb(RTC_DATA_REG, min))
		return 1;

	while(result & 0x80)
	{
		sys_outb(RTC_ADDR_REG, RTC_REG_A);
		sys_inb(RTC_DATA_REG, &result);
	}

	if(sys_outb(RTC_ADDR_REG, SEC))
		return 1;
	if(sys_inb(RTC_DATA_REG, sec))
		return 1;

	if(sys_outb(RTC_ADDR_REG, RTC_REG_B))
		return 1;
	if(sys_inb(RTC_DATA_REG, &result))
		return 1;

	if(!(result & 0x04))
	{
		*sec = rtc_bcd_to_bin(*sec);
		*min = rtc_bcd_to_bin(*min);
		*hour = rtc_bcd_to_bin(*hour);
	}

	rtc_ena_interrupts();
	return 0;
}

int rtc_get_date(unsigned long *dia, unsigned long *mes, unsigned long *ano)
{
	unsigned long result;

	rtc_dis_interrupts();

	if(sys_outb(RTC_ADDR_REG, MONTH_DAY))
		return 1;
	if(sys_inb(RTC_DATA_REG, dia))
		return 1;

	if(sys_outb(RTC_ADDR_REG, MONTH))
		return 1;
	if(sys_inb(RTC_DATA_REG, mes))
		return 1;

	if(sys_outb(RTC_ADDR_REG, YEAR))
		return 1;
	if(sys_inb(RTC_DATA_REG, ano))
		return 1;

	if(sys_outb(RTC_ADDR_REG, RTC_REG_B))
		return 1;
	if(sys_inb(RTC_DATA_REG, &result))
		return 1;

	if(!(result & 0x04))
	{
		*dia = rtc_bcd_to_bin(*dia);
		*mes = rtc_bcd_to_bin(*mes);
		*ano = rtc_bcd_to_bin(*ano);
	}
	rtc_ena_interrupts();
	return 0;
}

void rtc_show_time_and_date()
{
	unsigned long registers[4];
	unsigned long sec, min, hour, dia, mes, ano;

	rtc_get_time(&hour, &min, &sec);
	rtc_get_date(&dia, &mes,  &ano);

	printf("RTC:\n");

	if (((mes < 10) && (mes >= 0)) && ((dia < 10) && (dia >= 0)))
		printf("Date: 0%d-0%d-20%d\n", dia, mes, ano);
	else if((mes < 10) && (mes >= 0))
		printf("Date: %d-0%d-20%d\n", dia, mes, ano);
	else if((dia < 10) && (dia >= 0))
		printf("Date: 0%d-%d-20%d\n", dia, mes, ano);
	else
		printf("Date: %d-%d-20%d\n", dia, mes, ano);


	if (((sec < 10) && (sec >= 0)) && ((min < 10) && (min >= 0)) && ((hour <10) && (hour >= 0)))
		printf("Time: 0%d:0%d:0%d\n", hour, min, sec);
	else if(((sec < 10) && (sec >= 0)) && ((min < 10) && (min >= 0)))
		printf("Time: %d:0%d:0%d\n", hour, min, sec);
	else if(((sec < 10) && (sec >= 0)) && ((hour < 10) && (hour >= 0)))
		printf("Time: 0%d:%d:0%d\n", hour, min, sec);
	else if(((min < 10) && (min >= 0)) && ((hour < 10) && (hour >= 0)))
		printf("Time: 0%d:0%d:%d\n", hour, min, sec);
	else if ((sec < 10) && (sec >= 0))
		printf("Time: %d:%d:0%d\n", hour, min, sec);
	else if ((min < 10) && (min >= 0))
		printf("Time: %d:0%d:%d\n", hour, min, sec);
	else if ((hour < 10) && (hour >= 0))
		printf("Time: 0%d:%d:%d\n", hour, min, sec);
	else
		printf("Time: %d:%d:%d\n", hour, min, sec);

}

void rtc_update_date(Date * date)
{
	 rtc_get_time(&date->hours, &date->minutes, &date->seconds);
	 rtc_get_date(&date->day, &date->month, &date->year);
}
