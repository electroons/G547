/*
Kernel FIFO demo
EEE G547 - Lecture - 10
Author - Devesh Samaiya 
devesh@electroons.com
*/


#include<linux/init.h>
#include<linux/module.h>
#include<linux/kfifo.h>


struct kfifo fifo;
int ret;

static inline void empty_check(struct kfifo *fifo)
{
	printk(KERN_ALERT "Is it Empty?");	
	if(kfifo_is_empty(fifo))
		printk(KERN_ALERT "YES\n");
	else
		printk(KERN_ALERT "NO\n");	


}
int first_module_init(void)
{


	unsigned char val= 'A';
	unsigned int i;

	printk(KERN_ALERT "########## KFIFO DEMO MODULE ##########");

	ret = kfifo_alloc(&fifo, 16, GFP_KERNEL);
	if(ret)
		printk(KERN_ALERT "FIFO could not be allocated\n");
	else
		printk(KERN_ALERT "FIFO of %d Bytes Allocated Successfully, LEN = %d\n", kfifo_size(&fifo), kfifo_len(&fifo));

	empty_check(&fifo);


	while(!kfifo_is_full(&fifo))
	{
		if(kfifo_in(&fifo, &val, sizeof(val)))
			printk(KERN_ALERT "Enqueued %d bytes into FIFO,LEN=%d,val=%d", sizeof(val), kfifo_len(&fifo), val);
		else
			printk(KERN_ALERT "Could not insert into FIFO\n");
		val++;		
	}

	empty_check(&fifo);

	while(!kfifo_is_empty(&fifo))
	{
		if(kfifo_out(&fifo, &val, sizeof(val)))
			printk(KERN_ALERT "Dequeued %d bytes from FIFO,LEN=%d,val = %d", sizeof(val), kfifo_len(&fifo), val);
		else
			printk(KERN_ALERT "Could not pull from FIFO\n");
	}

	empty_check(&fifo);

	return 0;
}

int first_module_exit(void)
{
	printk(KERN_ALERT "Freeing FIFO, Leaving the kernel\n\n");
	kfifo_free(&fifo);
	return 0;
}

module_init(first_module_init);
module_exit(first_module_exit);




