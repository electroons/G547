#include<linux/init.h>
#include<linux/module.h>
#include<linux/sched.h>
#include<linux/delay.h>
#include<linux/interrupt.h>

static struct tasklet_struct tasklet;
static int dummy;
static unsigned int a = 6;

static irqreturn_t consumer(int irq, void *p_dummy)
{
	printk("inside interrupt handler\n");
	tasklet_schedule(&tasklet);
	
	return IRQ_NONE;
}

static void consumer_bh(unsigned long arg)
{
	unsigned int *q = (unsigned int*)arg;
	printk("Deferred function executed\n");
	mdelay(5000);
	printk("Arg = %u\n", *q);
	return; 
}

__init int firstmod_init(void)
{
	printk(KERN_ALERT "Tasklet Demo Module\n");

	tasklet_init(&tasklet, consumer_bh, (unsigned long)&a);

	if(request_irq(1, consumer, IRQF_SHARED, "sample", &dummy)<0)
			printk("interrupt installation failed\n");
	
	return 0;
}


__exit int firstmod_exit(void)
{
	printk(KERN_ALERT "Bye tasklet demo module\n\n");
	free_irq(1, &dummy);
	tasklet_kill(&tasklet)
	return 0;
}

module_init(firstmod_init);
module_exit(firstmod_exit);

MODULE_DESCRIPTION("First Module, EEE G547");
MODULE_AUTHOR("Devesh Samaiya <samaiya.devesh@gmail.com>");
MODULE_LICENSE("GPL");

