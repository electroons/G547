#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/stddef.h>
#include<linux/pci.h>
#include<linux/netdevice.h>
#include<linux/etherdevice.h>

#define REALTEK_VENDOR_ID 0x10EC
#define REALTEK_DEVICE_ID 0x8168

#define BCM4313_DEVICE_ID 0x4727
#define BROADCOM_VENDOR_ID 0x14e4
#define DRIVER "myeth"

struct pci_dev *pdev;
struct net_device *RTL8169;

struct rtl8169_private
{
	struct net_device_stats stats;
	struct pci_dev *pci_dev;
	struct net_device *dev;
};

static int RTL8169_open(struct net_device *dev)
{
	printk(KERN_ALERT "Inside RTL8169 Open Function\n\n");
	return 0;
}

static int RTL8169_release(struct net_device *dev)
{
	printk(KERN_ALERT "Inside RTL8169 Release Function\n\n");
	return 0;
}

static int RTL8169_xmit(struct sk_buff *skb,struct net_device *dev)
{
	printk(KERN_ALERT "Inside RTL8169 Transmit Function\n\n");
	return 0;
}

static struct net_device_stats* RTL8169_get_stats(struct net_device *dev)
{
	printk(KERN_ALERT "Inside RTL8169 Get Stats Function\n\n");
	return 0;
}

int RTL8169_init(struct net_device *dev)
{       	
	printk(KERN_ALERT "RTL8169 Initialized\n\n");
	return 0;
}

static const struct net_device_ops RTL8169_ops = {
	.ndo_open	= RTL8169_open,
	.ndo_stop	= RTL8169_release,
	.ndo_start_xmit = RTL8169_xmit,
	.ndo_get_stats64= RTL8169_get_stats,
};

int device_init(void)
{
	int result;

	struct rtl8169_private *tp;

	

	pdev = pci_get_device(REALTEK_VENDOR_ID, REALTEK_DEVICE_ID, NULL);

	if(pdev)
	{
		printk(KERN_ALERT "Device found\n\n");
		
		if(pci_enable_device(pdev))
		{
			printk(KERN_NOTICE "Could not enable the device\n");
			return NULL;
		}
		else
			printk(KERN_NOTICE "RTL8169 device is enabled\n");
	}
	else
		printk(KERN_ALERT "Device not found\n\n");
		
	RTL8169 = alloc_etherdev((sizeof(struct rtl8169_private)));
	RTL8169->netdev_ops = &RTL8169_ops;
	tp = netdev_priv(RTL8169);
	tp->dev = RTL8169;
	tp->pci_dev = pdev;
	memcpy(RTL8169->name, DRIVER, sizeof(DRIVER));


	if((result = register_netdev(RTL8169)))
		printk(KERN_ALERT "RTL8169 Driver: error %i registering device\n\n", result);					
	return 0;
}

int device_exit(void)
{
	pci_dev_put(pdev);
	//pci_disable_device(pdev);
	printk(KERN_ALERT "Leaving the kernel\n\n");
	return 0;
}

module_init(device_init);
module_exit(device_exit);
