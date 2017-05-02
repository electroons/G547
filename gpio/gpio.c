

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

static void
_gpioa_set(struct gpio_chip *chip,
           unsigned offset, int value)
{
	printk(KERN_INFO "set\n");
}

static int
_gpioa_get(struct gpio_chip *chip,
           unsigned offset)
{
   
   printk(KERN_INFO "GPIO GET INFO: %d", offset);

   return 0;
}

static int
_direction_output(struct gpio_chip *chip,
                  unsigned offset, int value)
{
        printk(KERN_ALERT "output");
  
   return 0;
}

static int
_direction_input(struct gpio_chip *chip,
                 unsigned offset)
{
        printk(KERN_ALERT "input");
   
   return 0;
}

static int device_init(void)
{
 
   chip.label = "gpio-xboard";
   chip.dev = NULL; // optional device providing the GPIOs
   chip.owner = THIS_MODULE; // helps prevent removal of modules exporting active GPIOs, so this is required for proper cleanup
   chip.base = -1; //let the kernel decide the gpio number for us. /sys/class/gpio/gpiochipX/
   chip.ngpio = 12; /* 12 GPIO pins, PD0, PD1, PD3, PD5, PD6, PD7 (1 - 6);
                             PB0, PB1, PB2, PB3, PB4, PB5 (7 - 12) */
   chip.can_sleep = false;

   chip.set = _gpioa_set;
   chip.get = _gpioa_get;

   chip.direction_input = _direction_input;
   chip.direction_output = _direction_output;
   //data->timeout = 100;

   if (gpiochip_add(&chip) < 0)
   {
	   printk(KERN_ALERT "Failed to add gpio chip");
	   return -ENODEV;
   }

	//gpio_export(1,1);
	printk(KERN_ALERT "Inserted");
   return 0;
}

static void device_exit(void)
{
   gpio_unexport(1);
   gpiochip_remove(&chip);
	printk(KERN_ALERT "removed");

   
}



module_init(device_init);
module_exit(device_exit);
