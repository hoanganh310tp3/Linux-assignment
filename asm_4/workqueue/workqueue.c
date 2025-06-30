#include <linux/module.h>    // used for module_init(), module_exit(), MODULE_LICENSE(), MODULE_DESCRIPTION(), MODULE_AUTHOR()
#include <linux/kernel.h>    // used for printk()
#include <linux/init.h>      // used for __init, __exit macros
#include <linux/workqueue.h> // used for work_struct, INIT_WORK(), schedule_work(), flush_work()
#include <linux/delay.h>     // used for msleep()

// Work structure with unique name
static struct work_struct test_work;

// Work handler function with unique name
static void test_work_handler(struct work_struct *work)
{
    printk(KERN_INFO "TEST_WQ: Work executing...\n");
    
    // Simulate some work
    msleep(1000);  // Sleep for 1 second
    
    printk(KERN_INFO "TEST_WQ: Work completed!\n");
}

// Module initialization
static int __init test_wq_init(void)
{
    printk(KERN_INFO "TEST_WQ: Module loaded\n");
    
    // Initialize work
    INIT_WORK(&test_work, test_work_handler);
    
    // Schedule work
    schedule_work(&test_work);
    
    return 0;
}

// Module cleanup
static void __exit test_wq_exit(void)
{
    printk(KERN_INFO "TEST_WQ: Module unloading...\n");
    
    // Wait for work to complete
    flush_work(&test_work);
    
    printk(KERN_INFO "TEST_WQ: Module unloaded\n");
}

module_init(test_wq_init);
module_exit(test_wq_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Test Workqueue");
MODULE_AUTHOR("AnhPH58");


