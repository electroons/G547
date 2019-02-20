#include<linux/init.h>
#include<linux/module.h>
#include<linux/sched.h>
#include<linux/semaphore.h>
#include<linux/kthread.h>
#include<linux/delay.h>
#include<linux/interrupt.h>

static struct task_struct *k1;
static int dummy;
unsigned char buffer=0;

static irqreturn_t consumer(int irq, void *p_dummy)
{
	printk("inside interrupt handler\n");
	if(buffer==1)		
	{	
		buffer=0;
		printk("consumed\n");
	}
	
	return IRQ_NONE;
}
static int producer(void *arg)
{
	//unsigned int i=10;
	printk("producer started PID= %d\n",current_thread_info()->task->pid);
	//while(i>0)
	//{
		
			
			
			disable_irq(17);
			printk("irq disabled\n");
			if(buffer==0)
			{
				buffer=1;
				printk("produced\n");
			}
			msleep(5000);
			enable_irq(17);
			printk("irq enabled\n");
			//i--;	
				
			
	//}	
	do_exit(0);
	return 0;
}


__init int firstmod_init(void)
{
	printk(KERN_ALERT "Interrupt Context Demo Module\n");
	if(request_irq(12, consumer, IRQF_SHARED, "sample", &dummy)<0)
			printk("interrupt installation failed\n");
	else
			k1 = kthread_run(producer, NULL, "kthread1");
	
	return 0;
}


__exit int firstmod_exit(void)
{
	printk(KERN_ALERT "Bye INT module\n\n");
	free_irq(12, &dummy);
	return 0;
}

module_init(firstmod_init);
module_exit(firstmod_exit);

MODULE_DESCRIPTION("First Module, EEE G547");
MODULE_AUTHOR("Devesh Samaiya <samaiya.devesh@gmail.com>");
MODULE_LICENSE("GPL");

