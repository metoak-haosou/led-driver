KERNELDIR_ARM := /home/wangjie/work/workspace/kernel/myir_kernel/xlnx-4.6
KERNELDIR_X86 := /usr/src/linux-source-4.8.0/linux-source-4.8.0
PWD :=$(shell pwd)
obj-m := led4.o
module_arm:
	$(MAKE) -C $(KERNELDIR_ARM) M=$(PWD) modules
module_x86:
	$(MAKE) -C $(KERNELDIR_X86) M=$(PWD) modules
cp:
	scp *ko root@192.168.80.10:/home/root/
clean:
	rm -rf *.o *~ core .depend .*.cmd  *.mod.c .tmp_versions modules.order  Module.symvers
distclean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order  Module.symvers