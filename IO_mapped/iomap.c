/*
Kernel module to explain IO mapped hardware access in x86 based systems
RTC is a legacy device which is now implemented as part of PCH Chipset in modern PCs
Check your chipset (use $ lshw) datasheet to know more about RTC registers and other details

Author - Devesh Samaiya <devesh@electroons.com>
Written as part of course EEE G547 - Linux Device Drivers
BITS Pilani, Pilani Campus, Rajasthan, India 
*/


#include <linux/module.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <asm/io.h>
 
 
#define RTC_index 	0x70
#define RTC_data 	0x71

#define SECONDS 	0x00
#define MINUTES 	0x02
#define HOURS 		0x04
#define DAY_WEEK 	0x06
#define DAY_MONTH 	0x07
#define MONTH 		0x08
#define YEAR 		0x09

#define REGA		0x0A
#define REGB		0x0B
#define REGC		0x0C
#define REGD		0x0D 

struct rtc_time
{
	u8 minute;
	u8 seconds;
	u8 hours;
	u8 day_of_week;
	u8 day_of_month;
	u8 month;
	u8 year;
};

unsigned char *day[] = {"0" ,"SUN" ,"MON", "TUE", "WED", "THU", "FRI", "SAT"};
unsigned char *month[] = {"0", "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};


int write_rtcTime(struct rtc_time *tm)
{
	/*Assert SET bit in REGB before setting time*/
	/* DM bit is 1 to select Binary mode*/
	/*See datasheet of MC146818A for details*/
	outb_p(REGB, RTC_index);
	outb_p(0x84, RTC_data);
	/*Update stopped now write time data*/

	outb_p(SECONDS, RTC_index);
	outb_p(tm->seconds, RTC_data);
       
	outb_p(MINUTES, RTC_index);
	outb_p(tm->minute, RTC_data);
       
	outb_p(HOURS, RTC_index);
	outb_p(tm->hours, RTC_data);
       
	outb_p(DAY_WEEK, RTC_index);
	outb_p(tm->day_of_week, RTC_data);
       
	outb_p(DAY_MONTH, RTC_index);
	outb_p(tm->day_of_month, RTC_data);
       
	outb_p(MONTH, RTC_index);
	outb_p(tm->month, RTC_data);
       
	outb_p(YEAR, RTC_index);
	outb_p(tm->year, RTC_data);

	/*Clear SET bit in REGB after setting time*/
	outb_p(REGB, RTC_index);
	outb_p(0x04, RTC_data);
       
	return 0;
}

struct rtc_time read_rtcTime(void)
{

	struct rtc_time tm;

	outb_p(SECONDS, RTC_index);
	tm.seconds = inb(RTC_data);

	outb_p(MINUTES, RTC_index);
	tm.minute = inb(RTC_data);

	outb_p(HOURS, RTC_index);
	tm.hours = inb(RTC_data); 

	outb_p(DAY_WEEK, RTC_index);
	tm.day_of_week = inb(RTC_data);

	outb_p(DAY_MONTH, RTC_index);
	tm.day_of_month = inb(RTC_data);

	outb_p(MONTH, RTC_index);
	tm.month = inb(RTC_data);
		
	outb(YEAR, RTC_index);
	tm.year = inb(RTC_data);

	outb(0x0d, RTC_index);

	return tm;
}

int __init  init_module()
{
	int i=0;
	struct rtc_time set_tm,tm;

	/*filling the time data to be written to RTC
	  this data will go to RTC via write_rtcTime	*/
	set_tm.minute =  8;
	set_tm.seconds = 0;
	set_tm.hours = 0x0C;
	set_tm.year = 17;
	set_tm.month = 2;
	set_tm.day_of_week = 4;
	set_tm.day_of_month = 15;

	/*Be careful before using write_rtcTime()
	It can result in unstable time clock in your computer
	 it Writes directly to RTC present as part of PCH Chipset*/

        //write_rtcTime(&set_tm);
	
		
		tm = read_rtcTime();
		write_rtcTime(&tm);
		printk(KERN_ALERT "%s, %d %s %d, %d:%d:%d\n\n", day[tm.day_of_week], tm.day_of_month, month[tm.month], tm.year, tm.hours, tm.minute, tm.seconds);
	
    return 0;
}
 
void  __exit cleanup_module()
{
	outb(0x0d, RTC_index);
}
 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Devesh Samaiya <devesh[at]electroons[dot]com>");
MODULE_DESCRIPTION("Low Level IO mapped access to RTC");
