obj-m := virtual_disk_dd.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all :
	make -C $(KDIR) M=$(PWD) modules
clean :
	rm -f *.ko
	rm -f *.o
	rm -f *.mod.*
	rm -f .*.cmd
