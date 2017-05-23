KERNELDIR_ARM := /home/wangjie/work/workspace/kernel/myir_kernel/kernel-qspi
KERNELDIR_X86 := /usr/src/linux-source-4.8.0/linux-source-4.8.0
PWD :=$(shell pwd)
obj-m := led7.o 
module_arm:
	$(MAKE) -C $(KERNELDIR_ARM) M=$(PWD) modules
	arm-linux-gnueabihf-gcc test.c -o test
module_x86:
	$(MAKE) -C $(KERNELDIR_X86) M=$(PWD) modules
cp:
	scp *ko test root@192.168.80.10:/home/root/
clean:
	rm -rf *.o *~ core .depend .*.cmd  *.mod.c .tmp_versions modules.order  Module.symvers
distclean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order  Module.symvers