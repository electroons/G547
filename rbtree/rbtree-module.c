#include<linux/init.h>
#include<linux/module.h>
#include<linux/slab.h>
#include<linux/rbtree.h>

struct mynode
{
	struct rb_node node;
	int node_number;
};

struct rb_root mytree = RB_ROOT;
struct mynode *tree_node;


int rbtree_search(struct rb_root *root, int key)
{
	struct rb_node *node = root->rb_node;
	
	while(node)
	{
		struct mynode *data = container_of(node, struct mynode, node);
		printk(KERN_INFO "Key : %d, Current Data : %d\n", key, data->node_number);
		if(key<data->node_number)
		{
			printk(KERN_INFO " %d < %d", key, data->node_number);			
			node = node->rb_right;
		}
		else if( key>data->node_number)
		{
			printk(KERN_INFO " %d > %d", key, data->node_number);
			node = node->rb_left;
		}
		else
			return data->node_number;
	}
	
	return 0;
}	

int rbtree_insert(struct rb_root *root, struct mynode *data)
{
	struct rb_node **new = &(root->rb_node), *parent = NULL;
	struct mynode *this;

	while(*new)
	{
		parent = *new;
		
		this = container_of(*new, struct mynode, node);
		int result = strcmp(data->node_number, this->node_number);

		if(this->node_number < data->node_number)
			new = &((*new)->rb_left);
		else if (this->node_number > data->node_number)
			new = &((*new)->rb_right);
		else
			return ;
	}

	/*Add new node and rebalance the tree. */
	rb_link_node(&data->node, parent, new);
	rb_insert_color(&data->node, root);

	return 0;
}

static inline void insert_node(int node_num)
{
	tree_node = kmalloc(sizeof(*tree_node), GFP_KERNEL);
	tree_node->node_number=node_num;
	
	if((rbtree_insert(&mytree,tree_node))==0)
		printk(KERN_ALERT "Node %d inserted", node_num);
	else
		printk(KERN_ALERT "Error inserting node %d",node_num);		
}


int first_module_init(void)
{
	struct mymode *t_node;

	/*Insertion order below should result in 
	in-order sequence 3,4,5,7,12,14,15,16,17,18 */	
		insert_node(4);
		insert_node(7);
		insert_node(12);
		insert_node(15);
		insert_node(3);
		insert_node(5);
		insert_node(14);
		insert_node(18);
		insert_node(16);
		insert_node(17);

	/* Search Examples */		
	if(rbtree_search(&mytree, 57))
		printk(KERN_INFO "Search Successful");
	else
		printk(KERN_INFO "Not found !!!");

	if(rbtree_search(&mytree, 12))
		printk(KERN_INFO "Search Successful");
	else
		printk(KERN_INFO "Not found !!!");
	
	/* in order traversal */ 
	for (t_node = rb_first(&mytree); t_node; t_node = rb_next(t_node)) 
	{
	    printk(KERN_INFO "in-order node no. = %d\n", rb_entry(t_node, struct mynode, node)->node_number);
	}


	return 0;
}

int first_module_exit(void)
{
	printk(KERN_ALERT "RBTREE, Leaving the kernel\n\n");
	return 0;
}

module_init(first_module_init);
module_exit(first_module_exit);

MODULE_AUTHOR("Devesh Samaiya <devesh@electroons.com>");
MODULE_DESCRIPTION("rbtree demonstration example for EEE G547 Course");
MODULE_LICENSE("GPL");












