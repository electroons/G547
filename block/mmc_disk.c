
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/slab.h>
#include<linux/blkdev.h>
#include<linux/genhd.h>
#include<linux/spinlock.h>

#define DEVICE_NAME "SD_CARD"
#define NR_OF_SECTORS 1024
#define SECTOR_SIZE 512
#define CARD_CAPACITY  (NR_OF_SECTORS*SECTOR_SIZE)
#define MAJOR_NO 125

int err =0;

struct blkdev_private{
	struct request_queue *queue;
	struct gendisk *gd;
	spinlock_t lock;
};

struct request *req;

static struct block_device_operations blkdev_ops =
{
	owner: THIS_MODULE,
	//open: blkdev_open,
	//release: blkdev_release
};


static struct blkdev_private *p_blkdev = NULL;

void mmc_request(struct request_queue *q)
{
	struct request *req;
	req = blk_fetch_request(q);
	while(req != NULL)
	{
		if(req == NULL || (req->cmd_type != REQ_TYPE_FS))
		{
			printk(KERN_INFO "non FS request");
			__blk_end_request_all(req, -EIO);
			continue;
		}
		printk(KERN_ALERT "Inside request function\n");
		printk(KERN_ALERT "Target Sector No. %d ", req->__sector);
		if ( ! __blk_end_request_cur(req, 0) ) {
			req = blk_fetch_request(q);
		}
	}
	
}

int block_init(void)
{
	struct gendisk *mmc_disk = NULL;

	err = register_blkdev(MAJOR_NO, "MMC DISK");
	if (err < 0) 
		printk(KERN_WARNING "DiskonRAM: unable to get major number\n");
	
	p_blkdev = kmalloc(sizeof(struct blkdev_private),GFP_KERNEL);
	
	if(!p_blkdev)
	{
		printk("ENOMEM  at %d\n",__LINE__);
		return 0;
	}
	memset(p_blkdev, 0, sizeof(struct blkdev_private));

	spin_lock_init(&p_blkdev->lock);

	p_blkdev->queue = blk_init_queue(mmc_request, &p_blkdev->lock);

	mmc_disk = p_blkdev->gd = alloc_disk(2);
	if(!mmc_disk)
	{
		kfree(p_blkdev);
		printk(KERN_INFO "alloc_disk failed\n");
		return 0;
	}
	

	mmc_disk->major = MAJOR_NO;
	mmc_disk->first_minor = 0;
	mmc_disk->fops = &blkdev_ops;
	mmc_disk->queue = p_blkdev->queue;
	mmc_disk->private_data = p_blkdev;
	strcpy(mmc_disk->disk_name, DEVICE_NAME);
	set_capacity(mmc_disk, CARD_CAPACITY);
	add_disk(mmc_disk);
	printk(KERN_INFO "Registered disk\n");
	
	return 0;	
}

void block_exit(void)
{
	struct gendisk *mmc_disk = p_blkdev->gd;
	del_gendisk(mmc_disk);
	blk_cleanup_queue(p_blkdev->queue);
	kfree(p_blkdev);
	return;
}


module_init(block_init);
module_exit(block_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SAMAIYA");
MODULE_DESCRIPTION("DISK");
