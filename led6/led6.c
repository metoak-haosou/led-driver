/*
 * led6.c
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
#define _NAME "led6"
#define BASE_IOPORT 906
#define MAX_IOPORT 1023
#define MIO0 BASE_IOPORT+0
#define MIO9 BASE_IOPORT+9
#define USER_LED_1 0
#define USER_LED_2 1
#define RGB 2
#define RGB_ADDR 0x43c00000
#define ON 1
#define OFF 0

static dev_t dev;
static struct cdev *cdev = NULL;
struct class *drv_class;
static char *pRGB_ADDR = NULL;


static int
open_led6(struct inode *inode, struct file *fp)
{
	printk(KERN_NOTICE"open\n");
	return 0;
}
//关闭文件函数
static int
release_led6(struct inode *inode, struct file *fp)
{
	printk(KERN_NOTICE"release\n");
	return 0;
}
//读函数
static ssize_t
read_led6 (struct file *fp, char __user *ubuf, size_t count, loff_t *ppos)
{
	printk(KERN_NOTICE"read\n");
	return 0;
}
//写函数
static ssize_t
write_led6 (struct file *fp, const char __user *ubuf, size_t count, loff_t *ppos)
{
	printk(KERN_NOTICE"write\n");
	return 0;
}
//io控制函数
static long
ioctl_led6 (struct file *fp, unsigned int cmd, unsigned long args)
{
	int arg;
	get_user(arg,(int *)args);
	//int val = (ioread8(pRGB_ADDR)&(~7))|(arg&7);
	printk("0x%x\n",arg);
	switch(cmd){
	case RGB:
		iowrite8(arg,pRGB_ADDR);
		break;
	default:
		return -1;
	}
	return 0;
}
//文件操作函数集
static struct file_operations fops_led6={
	.owner = THIS_MODULE,
	.open = open_led6,
	.release = release_led6,
	.read = read_led6,
	.write = write_led6,
	.unlocked_ioctl = ioctl_led6,
};

//初始化函数
static int __init
led6_init(void)
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
	cdev_init(cdev,&fops_led6);
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
	request_mem_region(RGB_ADDR,1,"RGB");
	pRGB_ADDR = ioremap(RGB_ADDR,1);
	iowrite8(0xff,pRGB_ADDR);
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
led6_exit(void)
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
		iounmap(pRGB_ADDR);
		release_mem_region(RGB_ADDR,1);
	}
}

//一些必要的信息的声明
MODULE_AUTHOR("haosou");
MODULE_VERSION("v1.0");
MODULE_DESCRIPTION("A test to char device driver");
MODULE_LICENSE("GPL");
//初始化函数和卸载函数的注册
module_init(led6_init);
module_exit(led6_exit);
