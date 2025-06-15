#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/device.h>

// class đại diện cho 1 nhóm thiết bị cùng loại


dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;

/*
** Function Prototypes
*/
static int      __init etx_driver_init(void);   // Unable to name driver_init because it conflict type with the kernel function with the same name
static void     __exit etx_driver_exit(void);
static int      dev_open(struct inode *inode, struct file *file);
static int      dev_release(struct inode *inode, struct file *file);
static ssize_t  dev_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  dev_write(struct file *filp, const char *buf, size_t len, loff_t * off);

// File operations
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
/*
 Module init function
*/
static int __init etx_driver_init(void)
{
    // Allocating Major number
    // 0 is the first argument, which is the starting minor number. 1 is the second argument, which is the number of devices we want to register.
    // alloc_chrdev_region is a function to allocate a range of char device numbers. It returns a dev_t type, which is a combination of major and minor numbers.
    // dev_t is a type that represents a device number in the kernel. It is a combination of major and minor numbers.
    // If we want to register more than one device, we can change the second argument to the number of devices we want to register.
    // If we want to register a single device, we can set the second argument to 1.
    // My_CharDevice is the name of the device, which is used to identify the device in the system. can appear in /dev directory.

    if((alloc_chrdev_region(&dev, 0, 1, "My_CharDevice")) < 0){
        pr_err("Cannot allocate major number..!!\n");
        return -1;
    }
    pr_info("Major = %d, Minor = %d\n", MAJOR(dev), MINOR(dev));

    // Creating cdev structure
    cdev_init(&etx_cdev, &fops);
    // etc_cdev.ops = &fops . replacing cdev_init, read more about struct cdev, cdev_init in bootlin.
    // cdev_init purpose initialize struct cdev. cdev_init is allocate file operation and reset component, avoiding error.


// cdev_add is adding device number (dev_t). 1 is representing one device. count in cdev is representing the number of device in cdev.
// In cdev_add, 1 is representing the start point . and if we want to add more than 1 device, we can change the count arguement.
// dev is the device number, which is allocated by alloc_chrdev_region. It is a major and minor number.
// 
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
    // Remove the device
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    
    // Unregister the device
    // báo xho kernel hủy number đấy đi, kernel có thể gán device number đó cho thiết bị khác

    unregister_chrdev_region(dev, 1);
    
    // Remove cdev
    // cdev là quản lý thiết bị, lưu thông tin của file operations và số lượng device còn lại trong hệ thôngs. Ta có thể xóa cdev trước khi xóa device.
    // unregister trước device destroy được ko.
    // etx_cdev chỉ là thằng trung gian giữa device và file operations. Nó không phải là device, nó chỉ là một cấu trúc để quản lý device. minhf có thẻ xóa device number trước khi xóa cdev.
    // nếu trong trường hợp có 10 thiết bị thì ko nên gọi cdev_del bới nó sẽ ngắt hết các liên kết với fops nếu ta chỉ muốn ngắt 2 trong 10 thiết bị.
    
    cdev_del(&etx_cdev);
    
    pr_info("Device Driver Removed...Done..!!\n");

}

module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AnhPH58");
MODULE_DESCRIPTION("Simple character device driver");

// tìm hiểu thêm về tool chain , kiến trúc chip.
// sudo ls -la /dev/
