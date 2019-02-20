#include<linux/init.h>
#include<linux/module.h>
#include<linux/sched.h>
#include<linux/delay.h>
#include<linux/interrupt.h>
#include<linux/workqueue.h>

struct my_work{
	struct delayed_work dwork;
	void *data;
}my_work;

static int dummy;
static unsigned int a = 6;
static struct workqueue_struct *myqueue = NULL;


static irqreturn_t consumer(int irq, void *p_dummy)
{
	printk("inside interrupt handler\n");
	queue_delayed_work(myqueue, &my_work.dwork, 10);
	
	return IRQ_NONE;
}

static void consumer_bh(struct work_struct *work)
{
	struct delayed_work *dwp = container_of(work, struct delayed_work, work); 
	struct my_work *mwp = container_of(dwp, struct my_work, dwork);

	unsigned int *q = mwp->data;
	printk("Deferred function executed\n");
	printk("Arg = %u\n", *q);
	return; 
}



__init int firstmod_init(void)
{
	printk(KERN_ALERT "Workqueue Demo Module\n");

	myqueue = create_workqueue("myqueue");
	my_work.data = &a;
	INIT_DELAYED_WORK(&my_work.dwork, consumer_bh);

	
	if(request_irq(1, consumer, IRQF_SHARED, "sample", &dummy)<0)
			printk("interrupt installation failed\n");
	
	return 0;
}


__exit int firstmod_exit(void)
{
	printk(KERN_ALERT "Bye workqueue demo module\n\n");
	free_irq(1, &dummy);
	cancel_delayed_work(&my_work.dwork);
	destroy_workqueue(myqueue);
	return 0;
}

module_init(firstmod_init);
module_exit(firstmod_exit);

MODULE_DESCRIPTION("First Module, EEE G547");
MODULE_AUTHOR("Devesh Samaiya <samaiya.devesh@gmail.com>");
MODULE_LICENSE("GPL");

