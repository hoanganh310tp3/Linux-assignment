
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
/* for tasklets API */
char tasklet_data[]="We use a string; but it could be pointer to a structure";
static struct tasklet_struct my_tasklet;
/* Tasklet handler, that just print the data */
void tasklet_work(unsigned long data)
{
    pr_info("%s\n", (char *)data);
}

static int __init my_init(void)
{
/*
* Schedule the handler.
* Tasklet are also scheduled from interrupt handler
*/
    pr_info("Tasklet module loaded.\n");
    tasklet_init(&my_tasklet, tasklet_work, (unsigned long)tasklet_data);
    tasklet_schedule(&my_tasklet);
    return 0;
}

static void my_exit(void){
    pr_info("Tasklet module exiting.\n");
    tasklet_kill(&my_tasklet);
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("AnhPH58");
MODULE_LICENSE("GPL");