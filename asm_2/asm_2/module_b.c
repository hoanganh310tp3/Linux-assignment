#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

void function_from_module_a(void);

static int __init module_b_init(void)
{
	printk(KERN_INFO "module b successfully loaded\n");
	function_from_module_a();
	return 0;
}

static void __exit module_b_exit(void)
{
	printk(KERN_INFO "module b successfully unloaded\n");
}

module_init(module_b_init);
module_exit(module_b_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AnhPH58");
MODULE_DESCRIPTION("IMPORTER");
