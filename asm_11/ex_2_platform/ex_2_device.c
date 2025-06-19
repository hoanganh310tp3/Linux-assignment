
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#define DRIVER_NAME "multi_char_dev"

static struct platform_device *multi_device;

static int __init platform_dev_init(void)
{
    multi_device = platform_device_register_simple(DRIVER_NAME, -1, NULL, 0);
    if (IS_ERR(multi_device)) {
        pr_err("Failed to register platform device\n");
        return PTR_ERR(multi_device);
    }

    pr_info("Platform device registered\n");
    return 0;
}

static void __exit platform_dev_exit(void)
{
    platform_device_unregister(multi_device);
    pr_info("Platform device unregistered\n");
}

module_init(platform_dev_init);
module_exit(platform_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AnhPH58");
MODULE_DESCRIPTION("Platform Device for Multi Character Driver");
