#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

/*Nếu không có class_create(), bạn không thể dùng device_create(), và vì vậy file /dev/etx_device sẽ không được tạo tự động. Bạn sẽ phải tạo thủ công bằng mknod.*/

// phải tự định nghĩa file permission của file trên kernel
// việc đọc và ghi thử các trường hợp driver chỉ cho phép đọc driver chỉ cho phép ghi ,.. 
// permission pahir do mình tưj thục hiện 

#define BUFFER_SIZE 1024 // Define the size of the kernel buffer

// struct hoanganh_dev {
//     dev_t dev = 0;
//     static struct class *dev_class;
//     static struct cdev etx_cdev;
//     static char kernel_buffer[BUFFER_SIZE]; // Kernel buffer to store data
//     static int buffer_len = 0;
// };


dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
static char kernel_buffer[BUFFER_SIZE]; // Kernel buffer to store data
static int buffer_len = 0;

/*
** Function Prototypes
*/
static int      __init etx_driver_init(void);   // Unable to name driver_init because it conflict type with the kernel function with the same name
static void     __exit etx_driver_exit(void);
static int      dev_open(struct inode *inode, struct file *file);
static int      dev_release(struct inode *inode, struct file *file);
static ssize_t  dev_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  dev_write(struct file *filp, const char *buf, size_t len, loff_t * off);
/*
cdev is the struct in linux kernel, representing for a character device. It is crucial linking file operations and driver character device. 
struct cdev:
kobj: research more
module, file operation . when init cdev 
*/
// File operations
static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .read       = dev_read,
    .write      = dev_write,
    .open       = dev_open,
    .release    = dev_release,
};

static char *devnode_fn(struct device *dev, umode_t *mode){
    if(mode) *mode = 0666;
    return NULL;
}

static int dev_open(struct inode *inode, struct file *file){
    pr_info("Driver Open Function Called ..!\n");
    return 0;
}

static int dev_release(struct inode *inode, struct file *file){
    pr_info("Driver Release Function Called ..!!\n");
    return 0;
}

/*
+) filp is kernel object, representing opened file.
+) buf is buffer in user space, storing the readed data
+) count is the maximum byte desired to be readed in buf
+) off is the started position which is readed in file, updated after reading file
==> ssize_t: return the sucessful readed bytes or error code 
*/
static ssize_t dev_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    if(*off >= buffer_len) {
        return 0; // End of file
    }
    
    if(len > buffer_len - *off) {
        len = buffer_len - *off; // Adjust length if it exceeds available data
    }
/*
buf: pointer to user space, where data is copied.
kernel_buffer + *off: pointer to kernel space, data provenance.
len: the ammount of data needed to be copied.
==> kernel read a block of bytes, starting from position *off, not one by one byte, different from userspace when using while is needed.
*/
    if(copy_to_user(buf, kernel_buffer + *off, len)) {
        return -EFAULT; // Error in copying data to user space
    }
    *off += len; // Update offset
    return len; // Return number of bytes read
}

static ssize_t dev_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    if(len > BUFFER_SIZE){
        len = BUFFER_SIZE; // Limit write size to buffer size
    }
// copy_from_user returns number of bytes that could not be copied. On success, this will be zero.

    if(copy_from_user(kernel_buffer, buf, len) != 0) {
        return -EFAULT; // Error in copying data from user space
    }

    buffer_len = len;
    kernel_buffer[len] = '\0'; // Null-terminate the string

    return len;
}
/*
 Module init function
*/
static int __init etx_driver_init(void)
{
    // Allocating Major number, register a range of char device numbers

    if((alloc_chrdev_region(&dev, 0, 1, "My_CharDevice")) < 0){
        pr_err("Cannot allocate major number..!!\n");
        return -1;
    }
    pr_info("Major = %d, Minor = %d\n", MAJOR(dev), MINOR(dev));

    // Creating cdev structure
    cdev_init(&etx_cdev, &fops);

    if((cdev_add(&etx_cdev,dev,1)) < 0){
        pr_err("Cannot add the device to the system..!!\n");
        goto r_class;
    }

    if(IS_ERR(dev_class = class_create("My_CharDeviceClass"))){
        pr_err("Cannot create the struct class..!!\n");
        goto r_class;
    }

    dev_class->devnode = devnode_fn;

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
    /*
    void unregister_chrdev_region(dev_t from, unsigned count)
{
	dev_t to = from + count;
	dev_t n, next;

	for (n = from; n < to; n = next) {
		next = MKDEV(MAJOR(n)+1, 0);
		if (next > to)
			next = to;
		kfree(__unregister_chrdev_region(MAJOR(n), MINOR(n), next - n));
	}
}
    */
    unregister_chrdev_region(dev, 1);
    // trả lại số thiết bị đã cấp phát
    
    // Remove cdev
    cdev_del(&etx_cdev); // dev, count is comprised in cdev.
    
    pr_info("Device Driver Removed...Done..!!\n");
    /*cdev_add() dùng để gắn cdev vào kernel và liên kết với số thiết bị (dev_t).

cdev_del() sẽ gỡ bỏ cdev khỏi hệ thống kernel.

unregister_chrdev_region() sẽ trả lại số thiết bị dev_t về cho kernel.*/
//sudo chmod 666 /dev/my_char_device 

}

module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AnhPH58");
MODULE_DESCRIPTION("Simple character device driver");