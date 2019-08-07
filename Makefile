CURRENT = $(shell uname -r)
KDIR = /lib/modules/$(CURRENT)/build 
PWD = $(shell pwd) 
obj-m := chrdev.o
default: 
	$(MAKE) -C $(KDIR) M=$(PWD) modules 
clean: 
	@rm -f *.o .*.cmd .*.flags *.mod.c *.order .cache.mk Module.symvers
	@rm -f .*.*.cmd *~ *.*~ TODO.* 
	@rm -fR .tmp* 
	@rm -rf .tmp_versions 