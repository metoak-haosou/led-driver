/*
 * led5.c
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
#define _NAME "led5"
#define BASE_IOPORT 906
#define MAX_IOPORT 1023
#define MIO0 BASE_IOPORT+0
#define MIO9 BASE_IOPORT+9
#define ON 0
#define OFF 1

static dev_t dev;
static struct cdev *cdev = NULL;
struct class *drv_class;


static int
open_led5(struct inode *inode, struct file *fp)
{
	printk(KERN_NOTICE"open\n");
	gpio_set_value(MIO0,0);
	gpio_set_value(MIO9,0);
	printk("MIO0=%d,MIO9=%d\n",gpio_get_value(MIO0),gpio_get_value(MIO9));
	return 0;
}
//关闭文件函数
static int
release_led5(struct inode *inode, struct file *fp)
{
	printk(KERN_NOTICE"release\n");
	gpio_set_value(MIO0,1);
	gpio_set_value(MIO9,1);
	return 0;
}
//读函数
static ssize_t
read_led5 (struct file *fp, char __user *ubuf, size_t count, loff_t *ppos)
{
	printk(KERN_NOTICE"read\n");
	return 0;
}
//写函数
static ssize_t
write_led5 (struct file *fp, const char __user *ubuf, size_t count, loff_t *ppos)
{
	printk(KERN_NOTICE"write\n");
	return 0;
}
//io控制函数
static long
ioctl_led5 (struct file *fp, unsigned int cmd, unsigned long args)
{
	int led;
	get_user(led,(int *)args);
	//printk("%d\n",led);
	//printk("%d\n",cmd);
	//if(led != 0 || led != 9)
	//	return -1;
	switch(cmd){
	case ON:
		gpio_set_value(led==0?MIO0:MIO9,ON);
		return ON;
	case OFF:
		gpio_set_value(led==0?MIO0:MIO9,OFF);
		return OFF;
	default:
		return -1;
	}
}
//文件操作函数集
static struct file_operations fops_led5={
	.owner = THIS_MODULE,
	.open = open_led5,
	.release = release_led5,
	.read = read_led5,
	.write = write_led5,
	.unlocked_ioctl = ioctl_led5,
};

//初始化函数
static int __init
led5_init(void)
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
	cdev_init(cdev,&fops_led5);
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
led5_exit(void)
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
module_init(led5_init);
module_exit(led5_exit);
