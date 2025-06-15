#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init test_module_init(void)
{
	printk(KERN_INFO "test module successfully loaded.\n");
	printk(KERN_WARNING "something went wrong!\n");
	printk(KERN_DEBUG "need debug!\n");
	printk(KERN_ERR "existed error!\n");
	return 0;
}

static void __exit test_module_exit(void)
{
	printk(KERN_INFO "test module successfully unloaded!\n");
}

module_init(test_module_init);
module_exit(test_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AnhPH58");
MODULE_DESCRIPTION("Display message kernel log levels");
