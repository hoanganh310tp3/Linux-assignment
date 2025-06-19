
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>

#define NUM_DEVICES 5
#define BUFFER_SIZE 1024
#define DEVICE_NAME "multi_dev"

static dev_t dev_number;
static struct cdev my_cdev[NUM_DEVICES];
static struct class *dev_class;

struct device_data {
    char buffer[BUFFER_SIZE];
    int size;
};

struct my_device {
    const char *name;
    uint8_t perm_flag;
    struct device_data data;
};

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
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_release,
    .read = dev_read,
    .write = dev_write,
};

static char *devnode_fn(struct device *dev, umode_t *mode) {
    if (mode) *mode = 0666;
    return NULL;
}

static int dev_open(struct inode *inode, struct file *file) {
    int minor = iminor(inode);
    file->private_data = &devices[minor];
    pr_info("Opening device: %s\n", devices[minor].name);
    return 0;
}

static int dev_release(struct inode *inode, struct file *file) {
    int minor = iminor(inode);
    pr_info("Releasing device: %s\n", devices[minor].name);
    return 0;
}

static ssize_t dev_read(struct file *file, char __user *buf, size_t len, loff_t *off) {
    struct my_device *dev = file->private_data;

    if (!(dev->perm_flag & 0x2))
        return -EPERM;

    if (*off >= dev->data.size)
        return 0;

    size_t to_copy = min(len, (size_t)(dev->data.size - *off));
    if (copy_to_user(buf, dev->data.buffer + *off, to_copy))
        return -EFAULT;

    *off += to_copy;
    return to_copy;
}

static ssize_t dev_write(struct file *file, const char __user *buf, size_t len, loff_t *off) {
    struct my_device *dev = file->private_data;

    if (!(dev->perm_flag & 0x1))
        return -EPERM;

    size_t to_copy = min(len, (size_t)(BUFFER_SIZE - *off));
    if (copy_from_user(dev->data.buffer + *off, buf, to_copy))
        return -EFAULT;

    *off += to_copy;
    dev->data.size = *off;
    return to_copy;
}

static int platform_driver_probe(struct platform_device *pdev)
{
    int ret;
    pr_info("Probing platform driver\n");

    ret = alloc_chrdev_region(&dev_number, 0, NUM_DEVICES, DEVICE_NAME);
    if (ret < 0) return ret;

    dev_class = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(dev_class)) {
        unregister_chrdev_region(dev_number, NUM_DEVICES);
        return PTR_ERR(dev_class);
    }

    dev_class->devnode = devnode_fn;

    for (int i = 0; i < NUM_DEVICES; ++i) {
        cdev_init(&my_cdev[i], &fops);
        cdev_add(&my_cdev[i], MKDEV(MAJOR(dev_number), i), 1);
        device_create(dev_class, NULL, MKDEV(MAJOR(dev_number), i), NULL, devices[i].name);
    }

    return 0;
}

static int platform_driver_remove(struct platform_device *pdev)
{
    for (int i = 0; i < NUM_DEVICES; ++i) {
        device_destroy(dev_class, MKDEV(MAJOR(dev_number), i));
        cdev_del(&my_cdev[i]);
    }

    class_destroy(dev_class);
    unregister_chrdev_region(dev_number, NUM_DEVICES);

    pr_info("Platform driver removed\n");
    return 0;
}

static struct platform_driver multi_char_driver = {
    .driver = {
        .name = "multi_char_dev",
        .owner = THIS_MODULE,
    },
    .probe = platform_driver_probe,
    .remove = platform_driver_remove,
};

module_platform_driver(multi_char_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AnhPH58");
MODULE_DESCRIPTION("Platform Driver for Multi Character Devices");
