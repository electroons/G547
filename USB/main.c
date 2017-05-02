#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/usb.h>

#define CARD_READER_VID  0x14cd
#define CARD_READER_PID  0x125d

#define SAMSUNG_MEDIA_VID  0x04e8
#define SAMSUNG_MEDIA_PID  0x6860

#define NXP_KEIL_VID 0xc251
#define NXP_MSD_PID 0x1303

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

static void usbdev_disconnect(struct usb_interface *interface)
{
	printk(KERN_INFO "USBDEV Device Removed\n");
	return;
}

static struct usb_device_id usbdev_table [] = {
	{USB_DEVICE(CARD_READER_VID, CARD_READER_PID)},
	{USB_DEVICE(SAMSUNG_MEDIA_VID, SAMSUNG_MEDIA_PID)},
	{USB_DEVICE(NXP_KEIL_VID, NXP_MSD_PID)},
	{} /*terminating entry*/	
};

static int usbdev_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	int i;
	unsigned char epAddr, epAttr;
	//struct usb_host_interface *if_desc;
	struct usb_endpoint_descriptor *ep_desc;
	
	if(id->idProduct == CARD_READER_PID)
	{
		printk(KERN_INFO "Camera Plugged in\n");
	}
	else if(id->idProduct == SAMSUNG_MEDIA_PID)
	{
		printk(KERN_INFO "Media Plugged in\n");
	}
	else if(id->idVendor == NXP_KEIL_VID)
	{
		printk(KERN_INFO "Blueboard Plugged in\n");
	}


	//if_desc = interface->cur_altsetting;
	printk(KERN_INFO "No. of Altsettings = %d\n",interface->num_altsetting);

	printk(KERN_INFO "No. of Endpoints = %d\n", interface->cur_altsetting->desc.bNumEndpoints);

	for(i=0;i<interface->cur_altsetting->desc.bNumEndpoints;i++)
	{
		ep_desc = &interface->cur_altsetting->endpoint[i].desc;
		epAddr = ep_desc->bEndpointAddress;
		epAttr = ep_desc->bmAttributes;
	
		if((epAttr & USB_ENDPOINT_XFERTYPE_MASK)==USB_ENDPOINT_XFER_BULK)
		{
			if(epAddr & 0x80)
				printk(KERN_INFO "EP %d is Bulk IN\n", i);
			else
				printk(KERN_INFO "EP %d is Bulk OUT\n", i);
	
		}

	}
	//this line causing error
	//p_usbdev_info->class = interface->cur_altsetting->desc.bInterfaceClass;
	
	printk(KERN_INFO "USB DEVICE CLASS : %x", interface->cur_altsetting->desc.bInterfaceClass);
	printk(KERN_INFO "USB DEVICE SUB CLASS : %x", interface->cur_altsetting->desc.bInterfaceSubClass);
	printk(KERN_INFO "USB DEVICE Protocol : %x", interface->cur_altsetting->desc.bInterfaceProtocol);

return 0;
}



/*Operations structure*/
static struct usb_driver usbdev_driver = {
	name: "usbdev",  //name of the device
	probe: usbdev_probe, // Whenever Device is plugged in
	disconnect: usbdev_disconnect, // When we remove a device
	id_table: usbdev_table, //  List of devices served by this driver
};


int device_init(void)
{
	usb_register(&usbdev_driver);
	return 0;
}

int device_exit(void)
{
	usb_deregister(&usbdev_driver);
	printk(KERN_NOTICE "Leaving Kernel\n");
	return 0;
}

module_init(device_init);
module_exit(device_exit);
MODULE_LICENSE("GPL");


