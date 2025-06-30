#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/device.h>

dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;

static int      __init etx_driver_init(void);
static void     __exit etx_driver_exit(void);
static int      dev_open(struct inode *inode, struct file *file);
static int      dev_release(struct inode *inode, struct file *file);
static ssize_t  dev_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  dev_write(struct file *filp, const char *buf, size_t len, loff_t * off);

static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .read       = dev_read,
    .write      = dev_write,
    .open       = dev_open,
    .release    = dev_release,
};

static int dev_open(struct inode *inode, struct file *file){
    pr_info("Driver Open Function Called ..!\n");
    return 0;
}

static int dev_release(struct inode *inode, struct file *file){
    pr_info("Driver Release Function Called ..!!\n");
    return 0;
}

static ssize_t dev_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    pr_info("Driver Read Function Called..!!!\n");
    return 0;
}

static ssize_t dev_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    pr_info("Driver Write Function Called..!!!\n");
    return len;
}

static int __init etx_driver_init(void)
{
    if((alloc_chrdev_region(&dev, 0, 1, "My_CharDevice")) < 0){
        pr_err("Cannot allocate major number..!!\n");
        return -1;
    }
    pr_info("Major = %d, Minor = %d\n", MAJOR(dev), MINOR(dev));

    cdev_init(&etx_cdev, &fops);

    if((cdev_add(&etx_cdev,dev,1)) < 0){
        pr_err("Cannot add the device to the system..!!\n");
        goto r_class;
    }

    if(IS_ERR(dev_class = class_create("My_CharDeviceClass"))){
        pr_err("Cannot create the struct class..!!\n");
        goto r_class;
    }

    if(IS_ERR(device_create(dev_class, NULL, dev, NULL, "my_char_device"))){
        pr_err("Cannot create the Device..!!\n");
        goto r_device;
    }

    pr_info("Device Driver Inserted...Done..!!\n");
    return 0;

r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev, 1);
    pr_err("Device Driver Inserted...Failed..!!\n");
    return -1;
}

static void __exit etx_driver_exit(void){
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    unregister_chrdev_region(dev, 1);
    cdev_del(&etx_cdev);
    pr_info("Device Driver Removed...Done..!!\n");
}

module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AnhPH58");
MODULE_DESCRIPTION("Simple character device driver");
