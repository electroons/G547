#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/sched.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Devesh Samaiya <devesh[at]electroons[dot]com>");
MODULE_DESCRIPTION("example for understanding task descriptor in linux kernel");

void device_init(void);
void device_exit(void);

module_init(device_init);
module_exit(device_exit);

void device_init(void)
{
	struct task_struct *task =  current_thread_info()->task;
	/*Line above can also be written as */ 
	//sturct task_struct *task = current
	printk(KERN_NOTICE "Current process : %s, PID: %d, State: %d", task->comm, task->pid, task->state);
	printk(KERN_NOTICE "Current process cpu : %d", current_thread_info()->cpu);
	
	do
	{
		task = task->parent;
		printk(KERN_NOTICE "Parent process: %s, PID: %d, State: %d",task->comm, task->pid, task->state);
	}while(task->pid != 0);
}

void device_exit(void)
{
	printk(KERN_NOTICE "task_struct example exit\n");
}
