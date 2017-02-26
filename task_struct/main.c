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

static const char state_name[] = TASK_STATE_TO_CHAR_STR;

void device_init(void)
{
	struct task_struct *task =  current_thread_info()->task;
	/*Above line can also be written as */ 
	//struct task_struct *task = current
	printk(KERN_NOTICE "current process name : %s", task->comm);
	printk(KERN_NOTICE "current process pid: %d",task->pid);
	printk(KERN_NOTICE "current process cpu : %d", current_thread_info()->cpu);
	printk(KERN_NOTICE "current process state : %c", state_name[task->state]);

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
