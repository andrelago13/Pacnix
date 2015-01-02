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
	unsigned long day;
	unsigned long month;
	unsigned long year;
	unsigned long hours;
	unsigned long minutes;
	unsigned long seconds;
} Date;


// Important ports

#define RTC_ADDR_REG 0x070		/**< @brief port that let's us access a paticular register */
#define RTC_DATA_REG 0x71		/**< @brief port that let's us read or write the contents accessed previously by the port 0x070 */

// Registers
#define SEC		        0		/**< @brief seconds address */
#define MINUTE		    2		/**< @brief minutes address */
#define HOUR		    4		/**< @brief hours address */
#define WEEK_DAY	    6		/**< @brief weekday address */
#define MONTH_DAY	    7		/**< @brief month day address */
#define MONTH		    8		/**< @brief month address */
#define YEAR		    9		/**< @brief year address */
#define RTC_REG_A      10		/**< @brief Register A address */
#define RTC_REG_B      11		/**< @brief Register B address */
#define RTC_REG_C      12		/**< @brief Register C address */
#define RTC_REG_D      13		/**< @brief Register D address */

// Functions implemented

/**
 * @brief Initialize the date on the game
 *
 * @return Return the initialized date
 */
Date * initialize_date();

/**
 * @brief Converts a number with 2 symbols BCD into his corresponding binary
 *
 * @param bincode_dec  bcd to be converted
 * @return Return the BCD on binary
 */
unsigned char rtc_bcd2bin(unsigned char bincode_dec);

/**
 * @brief Disables the RTC's interrupts
 *
 */
void rtc_dis_interrupts();

/**
 * @brief Enables the RTC's interrupts
 *
 */
void rtc_ena_interrupts();

/**
 * @brief Permit us retrieve the time
 *
 * Uses rtc_ena_interrupts and rtc_dis_interrupts to get access to the hour, minutes and seconds
 *
 * @param hour Address of memory position to be filled with the hour
 * @param min Address of memory position to be filled with the minutes
 * @param sec Address of memory position to be filled with the seconds
 * @return Return 0 upon success and non-zero otherwise
 */
int rtc_get_time(unsigned long *hour, unsigned long *min, unsigned long *sec);

/**
 * @brief Permit us retrieve the date
 *
 * Uses rtc_ena_interrupts and rtc_dis_interrupts to get access to the day, month and year
 *
 * @param dia Address of memory position to be filled with the dia
 * @param mes Address of memory position to be filled with the mes
 * @param ano Address of memory position to be filled with the year
 * @return Return 0 upon success and non-zero otherwise
 */
int rtc_get_date(unsigned long *dia, unsigned long *mes, unsigned long *ano);

/**
 * @brief Show time and date on text mode
 *
 */
void rtc_show_time_and_date();

/**
 * @brief Permit us update the date
 *
 * @param date Object date that will be updated
 */
void rtc_update_date(Date * date);



#endif // _RTC_H //
