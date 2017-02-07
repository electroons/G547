obj-m := fifo_demo.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
