#include<linux/init.h>
#include<linux/module.h>
#include<linux/sched.h>
#include<linux/semaphore.h>
#include<linux/kthread.h>
#include<linux/delay.h>
static struct semaphore sem;
static struct task_struct *k1, *k2;
unsigned char buffer=0;

static int producer(void *arg)
{
	unsigned int i=10;
	printk("producer started PID= %d\n",current_thread_info()->task->pid);
	while(i>0)
	{
		
			
			down(&sem);
			
			printk("producer acquired lock\n");
			if(buffer==0)
				buffer=1;
			msleep(10000);
			
			up(&sem);
			printk("producer released lock\n");
			i--;	
				
			
	}	
	do_exit(0);
	return 0;
}

static int consumer(void *arg)
{
	unsigned int i=10;
	printk("consumer started PID= %d\n",current_thread_info()->task->pid);
	while(i>0)
	{	
			down(&sem);

			printk("consumer acquired lock\n");
			if(buffer==1)			
				buffer=0;
			msleep(2000);
	
			up(&sem);
			printk("consumer released lock\n");
			i--;
	}
	
	do_exit(0);
	return 0;
}

__init int firstmod_init(void)
{
	printk(KERN_ALERT "Semaphore Demo Module\n");
	sema_init(&sem, 1);
	k1 = kthread_run(producer, NULL, "kthread1");
	k2 = kthread_run(consumer, NULL, "kthread2"); 

	return 0;
}


__exit int firstmod_exit(void)
{
	printk(KERN_ALERT "Bye Linux\n\n");
	return 0;
}

module_init(firstmod_init);
module_exit(firstmod_exit);

MODULE_DESCRIPTION("First Module, EEE G547");
MODULE_AUTHOR("Devesh Samaiya <samaiya.devesh@gmail.com>");
MODULE_LICENSE("GPL");

