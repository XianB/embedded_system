#obj-m := globalvar_proc.o
	obj-m += char_dev.o
#	obj-m := globalvar_array.o
	obj-m += char_dev_new.o
	KERNELDIR = /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
