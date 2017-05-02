#include <linux/init.h>// macros used to markup functions e.g. __init, __exit
#include <linux/module.h>// Core header for loading LKMs into the kernel
#include <linux/kernel.h>// Contains types, macros, functions for the kernel
#include <linux/device.h>// Header to support the kernel Driver Model
#include<linux/slab.h>
#include <linux/gpio.h> //for usb stuffs

MODULE_LICENSE("GPL");
MODULE_AUTHOR("G547");
MODULE_DESCRIPTION("led subsystem example");
MODULE_VERSION("0.1");

struct gpio_chip *chip;




static int my_gpio_request(struct gpio_chip *chip, unsigned offset)
{
	  printk(KERN_INFO "Request function called OFFSET = %d\n", offset);	
		return 0;
}

static int my_gpio_get_direction(struct gpio_chip *chip, unsigned offset)
{
	  printk(KERN_INFO "gpio get direction function called OFFSET = %d\n", offset);
		return 0;
}

static int my_gpio_direction_input(struct gpio_chip *chip, unsigned offset)
{
	  printk(KERN_INFO "gpio direction input function called OFFSET = %d\n", offset);
	return 0;
}

static int my_gpio_direction_output(struct gpio_chip *chip, unsigned offset, int value)
{
	  printk(KERN_INFO "gpio direction output function called : offset = %d, value = %d \n", offset, value);
	return 0;
}

static int my_gpio_get(struct gpio_chip *chip, unsigned offset)
{
	  printk(KERN_INFO "gpio get function called : offset = %d \n", offset);
	return 0;
}

static void my_gpio_set(struct gpio_chip *chip, unsigned offset, int value)
{
	  printk(KERN_INFO "gpio set function called : offset = %d, value = %d \n", offset, value);
	
}

static int device_init(void)
{
	int ret;

	chip = kzalloc(sizeof(struct gpio_chip), GFP_KERNEL);
	chip->label = "my_gpio";
	//chip->parent = NULL;
	chip->owner = THIS_MODULE;
	chip->request = my_gpio_request;
	chip->get_direction = my_gpio_get_direction;
	chip->direction_input = my_gpio_direction_input;
	chip->direction_output = my_gpio_direction_output;
	chip->get = my_gpio_get;
	chip->set = my_gpio_set;
	//chip->set_debounce = my_gpio_set_debounce;
	//chip->to_irq = my_gpio_to_irq;
	chip->base =-1;
	chip->ngpio = 4;
	chip->can_sleep = true;
	
	ret = gpiochip_add(chip);
	if(ret)
	   printk(KERN_INFO "failed to add GPIO device \n");

	printk(KERN_INFO "GPIO device loaded \n");
	return 0;
}

static int device_exit(void)
{

   	gpiochip_remove(chip);

	   printk(KERN_INFO "LED device unloaded \n");
	return 0;
}


module_init(device_init);
module_exit(device_exit);
