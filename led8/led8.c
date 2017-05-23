/*
 * led8.c
 *
 *  Created on: 2017-4-5
 *      Author: wangjie
 */


#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/bitrev.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/device.h>
#include <linux/ioport.h>
#include <asm/gpio.h>

#define _MIN 0
#define _MAJ 116
#define _NAME "led8"
#define BASE_IOPORT 906
#define MAX_IOPORT 1023
#define MIO0 BASE_IOPORT+0
#define MIO9 BASE_IOPORT+9
#define RGB_ADDR 0x43c00000
#define LEDDEV_IOC_MAGIC 'x'
#define LEDDEV_IOCUSERLED1 _IOW(LEDDEV_IOC_MAGIC,1,char)
#define LEDDEV_IOCUSERLED2 _IOW(LEDDEV_IOC_MAGIC,2,char)
#define LEDDEV_IOCRED _IOW(LEDDEV_IOC_MAGIC,3,char)
#define LEDDEV_IOCGREEN _IOW(LEDDEV_IOC_MAGIC,4,char)
#define LEDDEV_IOCBLUE _IOW(LEDDEV_IOC_MAGIC,5,char)
#define LEDDEV_IOC_MAXNR 5
#define ON 0
#define OFF 1

static dev_t dev;
static struct cdev *cdev = NULL;
struct class *drv_class;
static char *pRGB_ADDR = NULL;


static int
open_led8(struct inode *inode, struct file *fp)
{
	printk(KERN_NOTICE"open\n");
	iowrite8(ioread8(pRGB_ADDR)&(~0x7),pRGB_ADDR);//打开RGB三个灯
	gpio_set_value(MIO0,0);
	gpio_set_value(MIO9,0);
	return 0;
}

//关闭文件函数
static int
release_led8(struct inode *inode, struct file *fp)
{
	printk(KERN_NOTICE"release\n");
	iowrite8(ioread8(pRGB_ADDR)&0x7,pRGB_ADDR);//关闭RGB三个灯
	gpio_set_value(MIO0,1);
	gpio_set_value(MIO9,1);
	return 0;
}

//读函数
static ssize_t
read_led8 (struct file *fp, char __user *ubuf, size_t count, loff_t *ppos)
{
	printk(KERN_NOTICE"read\n");
	return 0;
}

//写函数
static ssize_t
write_led8 (struct file *fp, const char __user *ubuf, size_t count, loff_t *ppos)
{
	printk(KERN_NOTICE"write\n");
	return 0;
}

//io控制函数
static long
ioctl_led5 (struct file *fp, unsigned int cmd, unsigned long args)
{
	char arg;
	//检测命令的有效性
	if(_IOC_TYPE(cmd) != LEDDEV_IOC_MAGIC)
		return -EINVAL;
	if(_IOC_NR(cmd) > LEDDEV_IOC_MAXNR)
		return -EINVAL;
	//printk("cmd ok\n");
	//检测命令的类型，进而检测参数空间是否可用
	int err;
	if(_IOC_DIR(cmd)&_IOC_READ)
		err = !access_ok(VERIFY_READ,(void *)args,_IOC_SIZE(cmd));
	else if(_IOC_DIR(cmd)&_IOC_WRITE)
		err = !access_ok(VERIFY_WRITE,(void *)args,_IOC_SIZE(cmd));
	if(err)
		return -EINVAL;
	//printk("cmd access\n");

	get_user(arg,(char *)args);
	//printk("%d,%d\n",cmd,arg);
	switch(cmd){
	case LEDDEV_IOCUSERLED1:
		gpio_set_value(MIO0,arg == 0?ON:OFF);
		return LEDDEV_IOCUSERLED1;
	case LEDDEV_IOCUSERLED2:
		gpio_set_value(MIO9,arg == 0?ON:OFF);
		return LEDDEV_IOCUSERLED2;
	case LEDDEV_IOCRED:
		if(arg == ON)
			iowrite8(ioread8(pRGB_ADDR)&(~(1<<2)),pRGB_ADDR);
		else
			iowrite8(ioread8(pRGB_ADDR)|(1<<2),pRGB_ADDR);
		return LEDDEV_IOCRED;
	case LEDDEV_IOCGREEN:
		if(arg == ON)
			iowrite8(ioread8(pRGB_ADDR)&(~(1<<1)),pRGB_ADDR);
		else
			iowrite8(ioread8(pRGB_ADDR)|(1<<1),pRGB_ADDR);
		return LEDDEV_IOCGREEN;
	case LEDDEV_IOCBLUE:
		if(arg == ON)
			iowrite8(ioread8(pRGB_ADDR)&(~(1<<0)),pRGB_ADDR);
		else
			iowrite8(ioread8(pRGB_ADDR)|(1<<0),pRGB_ADDR);
		return LEDDEV_IOCBLUE;
	default:
		return -1;
	}
}

//文件操作函数集
static struct file_operations fops_led8={
	.owner = THIS_MODULE,
	.open = open_led8,
	.release = release_led8,
	.read = read_led8,
	.write = write_led8,
	.unlocked_ioctl = ioctl_led5,
};

//初始化函数
static int __init
led8_init(void)
{
	dev = MKDEV(_MAJ,_MIN);
	if(register_chrdev_region(dev,1,_NAME)){
		alloc_chrdev_region(&dev,_MIN,1,_NAME);
	}

	cdev = cdev_alloc();
	if (!cdev) {
		printk("at file %s  line %d function %s : cdev_alloc failed\n",__FILE__,__LINE__,__FUNCTION__);
		goto out1;
	}
	cdev_init(cdev,&fops_led8);
	if(cdev_add(cdev,dev,1)){
		printk("at file %s  line %d function %s : cdev_add failed\n",__FILE__,__LINE__,__FUNCTION__);
		goto out2;
	}

	drv_class = class_create(THIS_MODULE,_NAME);
	if(!drv_class){
		printk("class_create error\n");
		goto out3;
	}

	if (IS_ERR(device_create(drv_class, NULL,dev,NULL, _NAME))){
		printk("Could not create files\n");
		goto out4;
	}else
		printk("device_create success\n");

	gpio_request(MIO9,"MIO9");
	gpio_direction_output(MIO9,1);
	gpio_request(MIO0,"MIO0");
	gpio_direction_output(MIO0,1);

	request_mem_region(RGB_ADDR,1,"RGB");
	pRGB_ADDR = ioremap(RGB_ADDR,1);
	//u8 t = 0xff;
	iowrite8(ioread8(pRGB_ADDR)|0x7,pRGB_ADDR);//关闭RGB三个灯

	return 0;
out4:
	device_destroy(drv_class, dev);
out3:
	class_destroy(drv_class);
out2:
	cdev_del(cdev);
	cdev = NULL;
out1:
	unregister_chrdev_region(dev,1);
	return 0;
}

//卸载函数
static void __exit
led8_exit(void)
{
	if(cdev){
		//cdev的删除
		cdev_del(cdev);
		//反注册设备号
		unregister_chrdev_region(dev,1);
		printk(KERN_NOTICE"Goodbye %s\n","world");
		//设备文件的删除
		device_destroy(drv_class, dev);
		class_destroy(drv_class);//注意：这里必须device_destroy在前面
		gpio_free(MIO9);
		gpio_free(MIO0);

	}
}

//一些必要的信息的声明
MODULE_AUTHOR("haosou");
MODULE_VERSION("v1.0");
MODULE_DESCRIPTION("A test to char device driver");
MODULE_LICENSE("GPL");
//初始化函数和卸载函数的注册
module_init(led8_init);
module_exit(led8_exit);
