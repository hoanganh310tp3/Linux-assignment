#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/workqueue.h>

static DECLARE_WAIT_QUEUE_HEAD(my_wq);
static int sleep_flag = 0;

struct work_data {
    struct work_struct my_work;
    int the_data;
};

static void work_handler(struct work_struct *work)
{
    struct work_data *my_data = container_of(work, struct work_data, my_work);

    printk(KERN_INFO "Work queue handler: data = %d\n", my_data->the_data);

    msleep(2000);  // Giả lập công việc tốn thời gian

    sleep_flag = 1;
    wake_up_interruptible(&my_wq);

    kfree(my_data);
}

static int __init my_init(void)
{
    struct work_data *my_data;

    printk(KERN_INFO "Workqueue module init\n");

    my_data = kmalloc(sizeof(struct work_data), GFP_KERNEL);
    if (!my_data)
        return -ENOMEM;

    my_data->the_data = 34;

    INIT_WORK(&my_data->my_work, work_handler);
    schedule_work(&my_data->my_work);

    printk(KERN_INFO "Work scheduled, waiting for it to complete...\n");

    wait_event_interruptible(my_wq, sleep_flag != 0);

    printk(KERN_INFO "Work completed, init done.\n");
    return 0;
}

static void __exit my_exit(void)
{
    printk(KERN_INFO "Workqueue module exit\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AnhPH58");
MODULE_DESCRIPTION("Corrected shared workqueue example with wait queue");
