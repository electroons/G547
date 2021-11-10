#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/usb.h>
#include<linux/slab.h>

#define DIR_WRITE 1
#define DIR_READ 2

#define CARD_READER_VID 0x14cd
#define CARD_READER_PID 0x125d

#define SAMSUNG_MEDIA_VID 0x04e8
#define SAMSUNG_MEDIA_PID 0x6860

struct usbdev_private
{
	struct usb_device *udev;
	unsigned char class;
	unsigned char subclass;
	unsigned char protocol;
	unsigned char ep_in;
	unsigned char ep_out;
};

struct usbdev_private *p_usbdev_info;

static void usbdev_wakeup_on_completion(struct urb *urb);
static int usbdev_bulk_raw(int dir, struct usbdev_private *p_usbdev, unsigned char *data, unsigned int len);

int usbdev_bulk_data_out(struct usbdev_private *p_usbdev, unsigned char *src, unsigned int len)
{
	int status;
	printk(KERN_INFO "Enter: inside usbdev_bulk_data_out\n\n");

	if(len==0)
		return 0;
	
	status = usbdev_bulk_raw(DIR_WRITE, p_usbdev, src, len);

	printk(KERN_INFO "Exit: from usbdev_bulk_data_out\n\n");

	return status;
}

int usbdev_bulk_data_in(struct usbdev_private *p_usbdev, unsigned char *dest, unsigned int len)
{
	int status;
	printk(KERN_INFO "Enter: inside usbdev_bulk_data_IN\n\n");

	if(len==0)
		return 0;
	
	status = usbdev_bulk_raw(DIR_READ, p_usbdev, dest, len);

	printk(KERN_INFO "Exit: from usbdev_bulk_data_IN\n\n");

	return status;
}

//
//@dir - Direction of data transfer Write to disk or Read from Disk
//@*p_usbdev - pointer to private structure of usb device
//@data - Pointer to data to be transferred
//@len- length of data
//
static int usbdev_bulk_raw(int dir, struct usbdev_private *p_usbdev, unsigned char *data, unsigned int len)
{
	int pipe,result, len_done, halt_clearing = 0;
	struct completion *urb_done = NULL;
	struct urb *p_urb_request;
	unsigned char *dma_buf;

	urb_done = kmalloc(sizeof(struct completion), GFP_KERNEL);
	if(urb_done)
		init_completion(urb_done);
	else
	{
		printk(KERN_INFO " urb_done is NULL\n");
		return -1;
	}
	
	if(dir == DIR_READ)
		pipe = usb_rcvbulkpipe(p_usbdev->udev, p_usbdev->ep_in);
	else if(dir == DIR_WRITE)
		pipe = usb_sndbulkpipe(p_usbdev->udev, p_usbdev->ep_out);
	else
	{
		printk(KERN_INFO " Invalid USB request\n");
		return -1;
	}

	p_urb_request = usb_alloc_urb(0, GFP_KERNEL);
	if(p_urb_request == NULL)
	{
		printk(KERN_INFO "usb_alloc_urb failed \n");
		kfree(urb_done);
		return -1;
	}

	// allocating usb dma memory buffer
	dma_buf = usb_alloc_coherent(p_usbdev->udev, len, GFP_KERNEL, &p_urb_request->transfer_dma);
	if(dma_buf==NULL)
	{
		printk(KERN_INFO "DMA buffer alloc failed \n");
		usb_free_urb(p_urb_request);
		kfree(urb_done);
		return -1;
	}
	else
		memset(dma_buf, 0, len);

	if(dir==DIR_WRITE)
		memcpy(dma_buf,data,len);


	usb_fill_bulk_urb(p_urb_request, p_usbdev->udev, pipe, dma_buf, len, usbdev_wakeup_on_completion, urb_done);
	
	p_urb_request->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;

	result = usb_submit_urb(p_urb_request, GFP_KERNEL);
	if(result)
	{
		printk(KERN_ALERT "urb submit failed\n");
		usb_free_urb(p_urb_request);
		kfree(urb_done);
		return result;
	}

	wait_for_completion(urb_done);
	kfree(urb_done);
	len_done = p_urb_request->actual_length;
	result = p_urb_request->status;

	if(dir = DIR_READ)
		memcpy(data, dma_buf, len_done);

	//free usb dma memory
	usb_free_coherent(p_urb_request->dev,p_urb_request->transfer_buffer_length, p_urb_request->transfer_buffer, p_urb_request->transfer_dma );
	usb_free_urb(p_urb_request);

	// error handling 
	if(result)
	{
		if(result == -ETIMEDOUT)
			printk(KERN_ALERT "bulk_raw TIMEOUT\n");
		else if(result == -ENOENT)
			printk(KERN_ALERT "bulk_raw Transfer Aborted\n");
		else if(result == -EPIPE)
		{
			printk(KERN_ALERT "bulk_raw Pine Stalled\n");
			if(usb_clear_halt(p_usbdev->udev, pipe)<0)
				printk(KERN_ALERT "Halt could not be cleared\n");
		}
		return -1;
	}
	
	return 0;
}

static void usbdev_wakeup_on_completion(struct urb *urb)
{
	struct completion *urb_done_ptr;
	printk(KERN_INFO "ENTER : Wakeup on completion\n");
	urb_done_ptr = (struct cpmpletion *)urb->context;
	complete(urb_done_ptr);
	return; 
}


/*table of devices that work with this driver*/
static struct usb_device_id usbdev_table [] = {
	{USB_DEVICE(CARD_READER_VID,CARD_READER_PID)},
	{USB_DEVICE(SAMSUNG_MEDIA_VID,SAMSUNG_MEDIA_PID)},
	{}/*terminating entry*/
};	

/*gather all device related infor in probe*/
static int usbdev_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	struct usb_host_interface *if_desc;
	struct usb_endpoint_descriptor *ep_desc;
	int i;
	unsigned char epAddr, epAttr;

	/*Log information on device plug-in*/
	if(id->idProduct == CARD_READER_PID)
	{
		printk(KERN_INFO "Card Reader Plugged in\n");
	}
	else if(id->idProduct == CARD_READER_PID)
	{
		printk(KERN_INFO "Media Device Plugged in\n");
	}

	/*Checkout the endpoints*/
	if_desc = interface->cur_altsetting;
	
	printk(KERN_INFO "No. of altsettings = %d\n", interface->num_altsetting);
	printk(KERN_INFO "No. of endpoints = %d\n", if_desc->desc.bNumEndpoints);
	
	for(i=0; i <= if_desc->desc.bNumEndpoints; ++i)
	{
		ep_desc = &if_desc->endpoint[i].desc;
		epAddr = ep_desc->bEndpointAddress;
		epAttr = ep_desc->bmAttributes;
	
		if((epAttr & USB_ENDPOINT_XFERTYPE_MASK)==USB_ENDPOINT_XFER_BULK)
		{
			if(epAddr & 0x80)
			{
				printk(KERN_INFO "Endpoint %d is Bulk IN\n", i);
				p_usbdev_info->ep_in = ep_desc->bEndpointAddress;
			}
			else
			{
				printk(KERN_INFO "Endpoint %d is Bulk OUT\n", i);
				p_usbdev_info->ep_out = ep_desc->bEndpointAddress;
			}
		}
	}	
	
	//p_usbdev_info->class = if_desc->desc.bInterfaceClass;
	//p_usbdev_info->subclass = if_desc->desc.bInterfaceSubClass;
	//p_usbdev_info->protocol = if_desc->desc.bInterfaceProtocol;
 
	printk(KERN_INFO "USB Device Class: %x",if_desc->desc.bInterfaceClass);
	printk(KERN_INFO "USB Device Sub Class: %x",if_desc->desc.bInterfaceSubClass);
	printk(KERN_INFO "USB Device Protocol: %x",if_desc->desc.bInterfaceProtocol);

	//usbdev_bulk_data_out(p_usbdev_info, "Devesh Samaiya", 8);

	return 0;
}

static void usbdev_disconnect(struct usb_interface *interface)
{
	printk(KERN_INFO "Demo Driver Device Removed\n");
	return;
}


static struct usb_driver usbdev_driver = {
	name: "usbdev",
	probe: usbdev_probe,
	disconnect:  usbdev_disconnect,
	id_table: usbdev_table,
};

int device_init(void)
{
	usb_register(&usbdev_driver);				
	return 0;
}

void device_exit(void)
{
	usb_deregister(&usbdev_driver);				
}

module_init(device_init);
module_exit(device_exit);

MODULE_LICENSE("GPL");
