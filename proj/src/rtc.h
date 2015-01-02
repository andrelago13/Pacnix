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
	int hour;
	int minutes;
	int seconds;
} Date;








#endif _RTC_H
