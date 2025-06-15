#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

void function_from_module_a(void)
{
	printk(KERN_INFO "SUCESSFULLY EXPORTED\n");
}

EXPORT_SYMBOL(function_from_module_a);

static int __init module_a_init(void)
{
	printk(KERN_INFO "module a successfully loaded\n");
	return 0;
}

static void __exit module_a_exit(void)
{  
	printk(KERN_INFO "module a successfully unloaded\n");
}

module_init(module_a_init);
module_exit(module_a_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AnhPH58");
MODULE_DESCRIPTION("EXPORTER");
