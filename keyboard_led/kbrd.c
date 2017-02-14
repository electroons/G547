#include<linux/module.h>
#include<linux/init.h>
#include<linux/tty.h>
#include<linux/kd.h>
#include<linux/vt.h>
#include<linux/console_struct.h>
#include<linux/vt_kern.h>

MODULE_DESCRIPTION("Controlling Keyboard LEDs");
MODULE_AUTHOR("Samaiya");
MODULE_LICENSE("GPL");


struct tty_driver *my_driver;

#define ALL_LEDS_ON 0x07
#define RESTORE_LEDS 0xFF


static int __init kbleds_init(void)
{
	//int i;
	//printk(KERN_ALERT "kbleds : loading\n");
	//printk(KERN_ALERT "kbleds: fgconsole is %d\n",fg_console);

	my_driver = vc_cons[fg_console].d->port.tty->driver;
	
	//printk(KERN_ALERT "kbleds: tty driver magic %d\n", my_driver->magic);
	(my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED, ALL_LEDS_ON);
	
	return 0;
}

static void __exit kbleds_cleanup(void)
{

	printk(KERN_ALERT "Unloading....\n");
	(my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED, RESTORE_LEDS);
}

module_init(kbleds_init);
module_exit(kbleds_cleanup);

