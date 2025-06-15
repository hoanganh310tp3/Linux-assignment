#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
#include <linux/err.h>
 
#define mem_size        1024           //Memory Size

dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
uint8_t *kernel_buffer;

/* 
Function prototypes
*/
static int      __init etx_driver_init(void);
static void     __exit etx_driver_exit(void);
static int      dev_open(struct inode *inode, struct file *file);
static int      dev_release(struct inode *inode, struct file *file);
static ssize_t  dev_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  dev_write(struct file *filp, const char *buf, size_t len, loff_t * off);

/*
** File Operations structure
*/
static struct file_operations fops =
{
    .owner    = THIS_MODULE,
    .read     = dev_read,
    .write    = dev_write,
    .open     = dev_open,
    .release  = dev_release,
};

