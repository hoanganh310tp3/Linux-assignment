#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/init.h>

static int my_sensor_probe(struct platform_device *pdev){
    struct device_node *np = pdev->dev.of_node;
    u32 threshold;

    if (of_property_read_u32(np, "threshold", &threshold)){
        dev_err(&pdev->dev, "Failed to read threshold property\n");
        return -EINVAL;
    }

    dev_info(&pdev->dev, "Threshold is readed from device tree: %d\n", threshold);
    return 0;
}

static void my_sensor_remove(struct platform_device *pdev){
    dev_info(&pdev->dev, "Device removed\n");
}

static const struct of_device_id my_sensor_of_match[] = {
    { .compatible = "you,my-sensor", },
    { },
};

MODULE_DEVICE_TABLE(of, my_sensor_of_match);

static struct platform_driver my_sensor_driver = {
    .probe = my_sensor_probe,
    .remove = my_sensor_remove,
    .driver = {
        .name = "my-sensor",
        .of_match_table = my_sensor_of_match,
    },
};

module_platform_driver(my_sensor_driver);

MODULE_AUTHOR("AnhPH58");
MODULE_DESCRIPTION("Exercise 1");
MODULE_LICENSE("GPL");