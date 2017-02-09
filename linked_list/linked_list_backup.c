/*
Kernel inked list demo
EEE G547 - Lecture - 10
Author - Devesh Samaiya 
devesh@electroons.com
*/


#include<linux/init.h>
#include<linux/module.h>
#include<linux/list.h>
#include<linux/slab.h>


#define TRUE 1

struct fox
{
	unsigned int tail_length;
	unsigned int weight;
	bool is_awesome;
	struct list_head list;
};

int add_fox(struct list_head *head,unsigned int wt, unsigned int tail_len, bool awesome)
{
	struct fox *t_fox;

	t_fox = kmalloc(sizeof(*t_fox), GFP_KERNEL);
	t_fox->tail_length =tail_len;
	t_fox->weight =wt;
	t_fox->is_awesome = awesome;

	list_add(&t_fox->list,head);
	return 0;
}

int first_module_init(void)
{
	struct fox *head_fox, *ghost_fox;

	head_fox = kmalloc(sizeof(*head_fox), GFP_KERNEL);
	head_fox->tail_length = 10;
	head_fox->weight =200;
	head_fox->is_awesome = TRUE;

	INIT_LIST_HEAD(&head_fox->list);
	
	//adding 4 new nodes to the linked list
	add_fox(&head_fox->list, 10, 100, 1);
	add_fox(&head_fox->list, 20, 100, 1);
	add_fox(&head_fox->list, 30, 100, 1);
	add_fox(&head_fox->list, 40, 100, 1);

	// Demo of container_of() Macro, 
	// it returns pointer to the structure containing head_fox->list pointer
	ghost_fox = container_of(&head_fox->list,struct fox, list);
	printk(KERN_ALERT "head fox weight %d\n\n", ghost_fox->weight);

	//List all the entries of linked list with head_fox as head node
	list_for_each_entry(ghost_fox, &head_fox->list, list)
		printk(KERN_ALERT "Lomdi Weight is %d ", ghost_fox->weight);
	
	// Delete node next to head node
	list_del(head_fox->list.prev);
	
	
	// List specific content of every list node in reverse order 			
	list_for_each_entry(ghost_fox, &head_fox->list, list)
		printk(KERN_ALERT "Lomdi Weight is %d ", ghost_fox->weight);
	
	
	return 0;
}

int first_module_exit(void)
{
	printk(KERN_ALERT "Leaving this world\n\n");
	return 0;
}

module_init(first_module_init);
module_exit(first_module_exit);




