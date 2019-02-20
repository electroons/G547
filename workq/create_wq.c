#include<linux/init.h>
#include<linux/module.h>
#include<linux/sched.h>
#include<linux/delay.h>
#include<linux/interrupt.h>
#include<linux/workqueue.h>

struct my_work{
	struct work_struct work;
	void *data;
}my_work;

static int dummy;
static unsigned int a = 6;
static struct workqueue_struct *myqueue = NULL;


static irqreturn_t consumer(int irq, void *p_dummy)
{
	printk("inside interrupt handler\n");
	queue_work(myqueue, &my_work.work);
	
	return IRQ_NONE;
}

static void consumer_bh(struct work_struct *work)
{
	struct my_work *mwp = container_of(work, struct my_work, work);

	unsigned int *q = mwp->data;
	printk("Deferred function executed\n");
	msleep(50);
	printk("Arg = %u\n", *q);
	return; 
}



__init int firstmod_init(void)
{
	printk(KERN_ALERT "Workqueue Demo Module\n");

	myqueue = create_workqueue("myqueue");
	my_work.data = &a;
	INIT_WORK(&my_work.work, consumer_bh);

	
	if(request_irq(1, consumer, IRQF_SHARED, "sample", &dummy)<0)
			printk("interrupt installation failed\n");
	
	return 0;
}


__exit int firstmod_exit(void)
{
	printk(KERN_ALERT "Bye workqueue demo module\n\n");
	free_irq(1, &dummy);
	flush_workqueue(myqueue);
	destroy_workqueue(myqueue);
	return 0;
}

module_init(firstmod_init);
module_exit(firstmod_exit);

MODULE_DESCRIPTION("First Module, EEE G547");
MODULE_AUTHOR("Devesh Samaiya <samaiya.devesh@gmail.com>");
MODULE_LICENSE("GPL");

