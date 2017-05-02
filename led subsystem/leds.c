#include <linux/init.h>// macros used to markup functions e.g. __init, __exit
#include <linux/module.h>// Core header for loading LKMs into the kernel
#include <linux/kernel.h>// Contains types, macros, functions for the kernel
#include <linux/device.h>// Header to support the kernel Driver Model

//#include <linux/usb.h> //for usb stuffs
#include <linux/slab.h> //for

//#include <linux/workqueue.h> //for work_struct
#include <linux/leds.h> //for led

MODULE_LICENSE("GPL");
MODULE_AUTHOR("G547");
MODULE_DESCRIPTION("led subsystem example");
MODULE_VERSION("0.1");

     struct led_classdev led;
     uint8_t brightness;




static void
control_led(struct led_classdev *led,
                enum led_brightness brightness)
{
   if(brightness==0)
	printk(KERN_INFO "LED OFF\n");
   else if(brightness==1)	
	printk(KERN_INFO "LED ON\n");
}


static int device_init(void)
{

	led.name = kasprintf(GFP_KERNEL, "demo-led:g:dummy");
   	led.brightness_set = control_led;
   	led.brightness = LED_OFF;
   	led.max_brightness = 1;

	   //register led device class
	   led_classdev_register(NULL, &led);
	   printk(KERN_INFO "LED device connected \n");
	return 0;
}

static int device_exit(void)
{

   	led_classdev_unregister(&led);

	   printk(KERN_INFO "LED device unloaded \n");
	return 0;
}


module_init(device_init);
module_exit(device_exit);
