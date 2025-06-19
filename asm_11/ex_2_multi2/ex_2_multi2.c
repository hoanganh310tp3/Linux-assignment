#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define BUFFER_SIZE 1024
#define NUM_DEVICES 5

static dev_t dev_number;
static struct class *dev_class;
static struct cdev my_cdev[NUM_DEVICES];

struct device_data {
    char buffer[BUFFER_SIZE];
    int size;
};

struct my_device {
    const char *name;
    uint8_t perm_flag;
    struct device_data data;
};

/* Permission flags:
 * 0x0: no access
 * 0x1: write only
 * 0x2: read only
 * 0x3: read + write
 */
static struct my_device devices[NUM_DEVICES] = {
    { .name = "sensor0", .perm_flag = 0x1 },
    { .name = "sensor1", .perm_flag = 0x0 },
    { .name = "hello0",  .perm_flag = 0x2 },
    { .name = "hello1",  .perm_flag = 0x3 },
    { .name = "goodbye", .perm_flag = 0x1 },
};

// Forward declaration
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char __user *, size_t, loff_t *);

static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = dev_open,
    .read    = dev_read,
    .write   = dev_write,
    .release = dev_release,
};

static char *devnode_fn(const struct device *dev, umode_t *mode){
    if(mode != NULL) {*mode = 0666;}
    return NULL;
}

static int dev_open(struct inode *inode, struct file *file){
    int minor = iminor(inode);
    pr_info("Opening device: %s (minor %d)\n", devices[minor].name, minor);
    file->private_data = &devices[minor];
    return 0;
}

static int dev_release(struct inode *inode, struct file *file){
    int minor = iminor(inode);
    pr_info("Releasing device: %s (minor %d)\n", devices[minor].name, minor);
    return 0;
}

static ssize_t dev_read(struct file *file, char __user *buf, size_t len, loff_t *off){
    struct my_device *dev = file->private_data;

    if (!(dev->perm_flag & 0x2)) {
        pr_warn("Read not permitted on device: %s\n", dev->name);
        return -EPERM;
    }

    if (*off >= dev->data.size)
        return 0;

    size_t to_copy = min(len, (size_t)(dev->data.size - *off));

    if (copy_to_user(buf, dev->data.buffer + *off, to_copy))
        return -EFAULT;

    *off += to_copy;
    return to_copy;
}

static ssize_t dev_write(struct file *file, const char __user *buf, size_t len, loff_t *off){
    struct my_device *dev = file->private_data;

    if (!(dev->perm_flag & 0x1)) {
        pr_warn("Write not permitted on device: %s\n", dev->name);
        return -EPERM;
    }

    size_t to_copy = min(len, (size_t)(BUFFER_SIZE - *off));

    if (copy_from_user(dev->data.buffer + *off, buf, to_copy))
        return -EFAULT;

    *off += to_copy;
    dev->data.size = *off;
    return to_copy;
}
static int __init multi_driver_init(void){
    int ret;

    ret = alloc_chrdev_region(&dev_number, 0, NUM_DEVICES, DEVICE_NAME);
    if (ret < 0) {
        pr_err("Failed to allocate major number\n");
        return ret;
    }

    dev_class = class_create(DEVICE_NAME);
    if (IS_ERR(dev_class)) {
        unregister_chrdev_region(dev_number, NUM_DEVICES);
        pr_err("Cannot create device class\n");
        return PTR_ERR(dev_class);
    }

    dev_class->devnode = devnode_fn;

    for (int i = 0; i < NUM_DEVICES; i++) {
        cdev_init(&my_cdev[i], &fops);
        my_cdev[i].owner = THIS_MODULE;
        cdev_add(&my_cdev[i], MKDEV(MAJOR(dev_number), i), 1);
        device_create(dev_class, NULL, MKDEV(MAJOR(dev_number), i), NULL, devices[i].name);
    }

    pr_info("Multi Device Driver Inserted...Done!\n");
    return 0;
}

static void __exit multi_driver_exit(void){
    for (int i = 0; i < NUM_DEVICES; i++) {
        device_destroy(dev_class, MKDEV(MAJOR(dev_number), i));
        cdev_del(&my_cdev[i]);
    }

    class_destroy(dev_class);
    unregister_chrdev_region(dev_number, NUM_DEVICES);
    pr_info("Multi Device Driver Unloaded.\n");
}

module_init(multi_driver_init);
module_exit(multi_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AnhPH58");
MODULE_DESCRIPTION("Multi character device driver with permission flags");
