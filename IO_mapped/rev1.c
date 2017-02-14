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
#define YEAR 		0x09
#define MONTH 		0x08

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

unsigned char *day[] = {"0" "SUN" "MON", "TUE", "WED", "THU", "FRI", "SAT"};
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

struct rtc_time rtc_readTime(void)
{

	struct rtc_time tm;

	outb_p(SECONDS, RTC_index);
	tm.seconds = inb(RTC_data);

	outb_p(MINUTES, RTC_index);
	tm.minute = inb(RTC_data);
	//printk(KERN_ALERT "MM: %d\n", inb(0x71));	

	outb_p(HOURS, RTC_index);
	tm.hours = inb(RTC_data); 
	//printk(KERN_ALERT "HR: %d\n", inb(0x71));

	outb_p(DAY_WEEK, RTC_index);
	tm.day_of_week = inb(RTC_data);
	//printk(KERN_ALERT "DAY_WEEK: %d\n", inb(0x71));        

	outb_p(DAY_MONTH, RTC_index);
	tm.day_of_month = inb(RTC_data);
	//printk(KERN_ALERT "DAY_MONTH: %d\n", inb(0x71));

	outb_p(MONTH, RTC_index);
	tm.month = inb(RTC_data);
	//printk(KERN_ALERT "MONTH: %d\n", inb(0x71));
		
	outb(YEAR, RTC_index);
	tm.year = inb(RTC_data);
	//printk(KERN_ALERT "YY: %d\n\n\n", inb(0x71));

	outb(0x0d, RTC_index);

	return tm;
}

int __init  init_module()
{
	int i=0;
	struct rtc_time set_tm,tm;
	set_tm.minute = 49;
	set_tm.seconds = 0;
	set_tm.hours = 1;
	set_tm.year = 17;
	set_tm.month = 2;
	set_tm.day_of_week = 3;
	set_tm.day_of_month = 15;

        //write_rtcTime(&set_tm);
	//while(i<100)
	//{
		//msleep(1000);
		tm = rtc_readTime();
		printk(KERN_ALERT "%s, %d %s %d, %d:%d:%d", day[tm.day_of_week], tm.day_of_month, month[tm.month], tm.year, tm.hours, tm.minute, tm.seconds);
		//i++;
	//}        
  
    return 0;
}
 
void  __exit cleanup_module()
{
	outb(0x0d, RTC_index);
}
 
//module_init(init_module);
//module_exit(cleanup_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Devesh Samaiya <devesh[at]electroons[dot]com>");
MODULE_DESCRIPTION("Low Level IO mapped access to RTC");
