ifneq ($(KERNELRELEASE),)
#obj-m := globalvar_proc.o
	obj-m := globalvar.o
	obj-m := globalvar_array.o
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
endif
