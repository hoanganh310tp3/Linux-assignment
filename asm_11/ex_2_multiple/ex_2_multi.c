#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define BUFFER_SIZE 1024
#define NUM_DEVICES 5
#define DEVICE_NAME "multi_dev"

static dev_t dev_number;
static struct class *dev_class;
static struct cdev my_cdev[NUM_DEVICES];

struct device_data {
    char buffer[BUFFER_SIZE];
    int size;
};

static struct device_data dev_data[NUM_DEVICES];

static int __init multi_driver_init(void);
static void __exit multi_driver_exit(void);
static int      dev_open(struct inode *inode, struct file *file);
static int      dev_release(struct inode *inode, struct file *file);
static ssize_t  dev_read(struct file *file, char __user *buf, size_t len,loff_t * off);
static ssize_t  dev_write(struct file *file, const char *buf, size_t len, loff_t * off);

static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .read       = dev_read,
    .write      = dev_write,
    .open       = dev_open,
    .release    = dev_release,
};

/* This prototype is taken in struct class to give file permissions */
static char *devnode_fn(const struct device *dev, umode_t *mode){
    if(mode != NULL) {*mode = 0666;}
    return NULL;
}

static int dev_open(struct inode *inode, struct file *file){
    int minor = iminor(inode); // Taking minor number of a device from struct inode
    pr_info("Opening device minor: %d\n", minor);
    file->private_data = &dev_data[minor];
    return 0;
}

static int dev_release(struct inode *inode, struct file *file){
    int minor = iminor(inode);
    pr_info("Closing device minor: %d\n", minor);
    file->private_data = &dev_data[minor];
    return 0;
}

static ssize_t dev_read(struct file *file, char __user *buf, size_t len, loff_t *off)
{
    pr_info("Reading device minor!!!\n");
    struct device_data *data = file->private_data;
    size_t to_copy;

    if(*off >= data->size) {
        return 0; 
    }

    to_copy = min(len, (size_t)(data->size - *off)); // Not reading data exceeding len and existed data in buffer
    
    if(copy_to_user(buf, data->buffer + *off, to_copy)) {
        return -EFAULT; 
    }
    *off += to_copy; 
    return to_copy; 
}

static ssize_t dev_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
     pr_info("Writing to device minor!!!n");
    struct device_data *data = file->private_data;
    size_t to_copy = min(len, (size_t)(BUFFER_SIZE - *off));

    if (copy_from_user(data->buffer + *off, buf, to_copy)){
        return -EFAULT;
    }

    *off += to_copy;
    data->size = *off;
    return to_copy;
}


static int __init multi_dev_init(void)
{
    int ret, i;

    ret = alloc_chrdev_region(&dev_number, 0, NUM_DEVICES, DEVICE_NAME);
    if(ret < 0){
        return ret;
    }

    dev_class = class_create(DEVICE_NAME);

    if(IS_ERR(dev_class)){
        pr_err("Cannot create the struct class..!!\n");
        goto r_class;
    }
    dev_class->devnode = devnode_fn;
    
    for (i = 0; i < NUM_DEVICES; i++){
        cdev_init(&my_cdev[i], &fops);
        my_cdev[i].owner = THIS_MODULE;
        cdev_add(&my_cdev[i], MKDEV(MAJOR(dev_number), i), 1);
        device_create(dev_class, NULL, MKDEV(MAJOR(dev_number),i), NULL, "multi_dev%d", i);
    }

    pr_info("Multi Device Driver Inserted...Done..!!\n");
    return 0;

r_class:
    unregister_chrdev_region(dev_number, NUM_DEVICES);
    pr_err("Multi Device Driver Inserted...Failed..!!\n");
    return -1;
}

static void __exit multi_dev_exit(void){
    int i;
    for(i = 0; i < NUM_DEVICES; i++){
        device_destroy(dev_class, MKDEV(MAJOR(dev_number), i));
        cdev_del(&my_cdev[i]);
    }

    class_destroy(dev_class);
    unregister_chrdev_region(dev_number, NUM_DEVICES);
    pr_info("Multi Device Driver unloaded");
}

module_init(multi_dev_init);
module_exit(multi_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AnhPH58");
MODULE_DESCRIPTION("Multi character device driver");






