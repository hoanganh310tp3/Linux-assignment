
#include <linux/kernel.h> // used for pr_info
#include <linux/module.h> // used for module_init(), module_exit()
#include <linux/interrupt.h> // used for tasklet_struct, tasklet_init(), tasklet_schedule(), tasklet_kill()

#define MY_IRQ 1 // IRQ 1 is used for keyboard

char tasklet_data[]="Tasklet is executed from interrupt !!!";
static struct tasklet_struct my_tasklet;
/* Tasklet handler, that just print the data */
void tasklet_work(unsigned long data)
{
    pr_info("%s\n", (char *)data);
}

irqreturn_t interrupt_handler(int irq, void *dev_id){
    pr_info("Interrupt %d is received !!!\n", irq);

    tasklet_schedule(&my_tasklet);
    return IRQ_HANDLED;
}

static int __init my_init(void)
{
    int ret;

    pr_info("Tasklet module loaded.\n");

    //Initialize tasklet with function handler
    tasklet_init(&my_tasklet, tasklet_work, (unsigned long)tasklet_data);

    ret = request_irq(MY_IRQ, interrupt_handler, IRQF_SHARED, "irq_handler", (void *)(interrupt_handler));
    if(ret){
        pr_err("Failed to register IRQ %d\n", MY_IRQ);
        return ret;
    }
    
    pr_info("Successfully registered IRQ %d\n", MY_IRQ);
    return 0;
}

static void my_exit(void){
    pr_info("Tasklet module exiting.\n");
    tasklet_kill(&my_tasklet);
    free_irq(MY_IRQ, (void *)(interrupt_handler));
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("AnhPH58");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Tasklet is triggered by interrupt handler");