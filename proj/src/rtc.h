#ifndef _RTC_H
#define _RTC_H

/** @defgroup rtc RTC
 * @{
 * @brief Functions for working with the PC's RTC
 */

/**@brief Struct representing a complete Date
 *
 * Contains the information of a date, such as day, month, year,
 * hour, minutes and seconds
 */
typedef struct {
	int day;
	int month;
	int year;
	int hours;
	int minutes;
	int seconds;
} Date;


// Important ports

#define RTC_ADDR_REG 0x070		/**< @brief clock frequency for timer in PC and AT */
#define RTC_DATA_REG 0x71

#define RTC_IRQ 8

// Registers
#define SEC		        0
#define MINUTE		    2
#define HOUR		    4
#define WEEK_DAY	    6
#define MONTH_DAY	    7
#define MONTH		    8
#define YEAR		    9
#define RTC_REG_A      10
#define RTC_REG_B      11
#define RTC_REG_C      12
#define RTC_REG_D      13

// Functions implemented


unsigned char rtc_bcd_to_bin(unsigned char bincode_dec);		/// Converte o um número com 2 símbolos BCD no numero binário correspondente

void rtc_dis_interrupts(); 			/// Disables rtc interrupts

void rtc_ena_interrupts();			/// Enables rtc interrupts

int rtc_get_time(unsigned long *hour, unsigned long *min, unsigned long *sec); 		/// Gets rtc time

int rtc_get_date(unsigned long *dia, unsigned long *mes, unsigned long *ano);		/// Gives rtc date

void rtc_show_time_and_date();

void rtc_update_date(Date * date);



#endif // _RTC_H //
