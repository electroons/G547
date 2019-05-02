#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>            /* For IRQ */
//#include <linux/of.h>                   /* For DT*/
#include <linux/iio/iio.h>    /* mandatory */
#include <linux/iio/sysfs.h>  /* mandatory since sysfs are used */
#include <linux/iio/events.h> /* For advanced users, to manage iio events */
#include <linux/iio/buffer.h>  /* mandatory to use triggered buffers */


#define FAKE_VOLTAGE_CHANNEL(num)   \
	{                               \
	    .type = IIO_VOLTAGE,        \
	    .indexed = 1,               \
	    .channel = (num),           \
	    .address = (num),           \
	    .info_mask_separate = BIT(IIO_CHAN_INFO_RAW),           \
	    .info_mask_shared_by_type = BIT(IIO_CHAN_INFO_SCALE)    \
	}

struct iio_dev *indio_dev1;
	

struct my_private_data {
	int foo;
	int bar;
	struct mutex lock;
};

static int fake_read_raw(struct iio_dev *indio_dev,
			    struct iio_chan_spec const *channel, int *val,
			    int *val2, long mask)
{
	printk("channel %d read\n",channel->channel);
	return 0;
}

static int fake_write_raw(struct iio_dev *indio_dev,
			    struct iio_chan_spec const *chan,
			    int val, int val2, long mask)
{
	printk("channel %d write\n",chan->channel);
	return 0;
}

static const struct iio_chan_spec fake_channels[] = {
	FAKE_VOLTAGE_CHANNEL(0),
	FAKE_VOLTAGE_CHANNEL(1),
	FAKE_VOLTAGE_CHANNEL(2),
	FAKE_VOLTAGE_CHANNEL(3),
};


static const struct iio_info fake_iio_info = {
	.read_raw = fake_read_raw,
	.write_raw= fake_write_raw,
	.driver_module = THIS_MODULE,
};

int my_iio_init(void)
{
	struct my_private_data *data;

	indio_dev1 = iio_device_alloc(sizeof(*data));
	if (!indio_dev1) {
		printk("iio allocation failed!\n");
		return -ENOMEM;
	}

	data = iio_priv(indio_dev1);
	mutex_init(&data->lock);
	indio_dev1->dev.parent = NULL;
	indio_dev1->info = &fake_iio_info;
	indio_dev1->name = "myiio";
	indio_dev1->modes = INDIO_DIRECT_MODE;
	indio_dev1->channels = fake_channels;
	indio_dev1->num_channels = 4;
	indio_dev1->available_scan_masks = 0x0F;
	iio_device_register(indio_dev1);
	return 0;
}

static void my_iio_remove(void)
{
	iio_device_unregister(indio_dev1);
	iio_device_free(indio_dev1);
}

module_init(my_iio_init);
module_exit(my_iio_remove);
MODULE_LICENSE("GPL");
