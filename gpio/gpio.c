

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/device.h>
#include <linux/usb.h>
#include <linux/slab.h>
#include <linux/gpio.h>



MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("G547");
MODULE_DESCRIPTION("gpio-12 driver.");
MODULE_VERSION("0.1");

   struct gpio_chip chip; //this is our GPIO chip
const char *gpionames[2] = {"LED", "RELAY"};

static void _gpioa_set(struct gpio_chip *chip, unsigned offset, int value)
{
	printk(KERN_INFO "SET %d \n", offset);
}

static void _gpioa_request(struct gpio_chip *chip, unsigned offset)
{
	printk(KERN_INFO "REQUESTED %d \n", offset);
}

static int _gpioa_get(struct gpio_chip *chip, unsigned offset)
{
   
   printk(KERN_INFO "GPIO GET INFO: %d", offset);
   return 0;
}

static int _direction_output(struct gpio_chip *chip, unsigned offset, int value)
{
        printk(KERN_ALERT "set offset %d output\n", (int)offset);
  	return 0;
}

static int _direction_input(struct gpio_chip *chip, unsigned offset)
{
        printk(KERN_ALERT "set offset %d input", (int)offset);
        return 0;
}

static int device_init(void)
{
 
   chip.label = "gpio-xboard";
   chip.dev = NULL; // optional device providing the GPIOs
   chip.owner = THIS_MODULE; // helps prevent removal of modules exporting active GPIOs, so this is required for proper cleanup
   chip.base = 0; 
   chip.ngpio = 10; 
   chip.can_sleep = false;

   chip.set = _gpioa_set;
   chip.get = _gpioa_get;
   chip.names = gpionames;
   chip.request = _gpioa_request; 	 	 
   chip.direction_input = _direction_input;
   chip.direction_output = _direction_output;
   

   if (gpiochip_add(&chip) < 0)
   {
	   printk(KERN_ALERT "Failed to add gpio chip");
	   return -ENODEV;
   }
   
  	gpio_request(0, NULL);
  	gpio_request(1, NULL);
   	gpio_export(0,1);
   	gpio_export(1,0);
   printk(KERN_ALERT "Inserted");
   return 0;
}

static void device_exit(void)
{
   //gpio_unexport(0);
   //gpio_unexport(1);
   //gpio_free(0);
   //gpio_free(1);
   gpiochip_remove(&chip);
   printk(KERN_ALERT "removed");

   
}



module_init(device_init);
module_exit(device_exit);
