#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>

static int my_probe(struct platform_device *pdev)
{
    struct device_node *np = pdev->dev.of_node;
    struct device_node *child;
    const char *label, *child_name;
    u32 threshold, speed;
    int len, i;
    const __be32 *vals;

    of_property_read_u32(np, "threshold", &threshold);
    of_property_read_string(np, "label", &label);
    vals = of_get_property(np, "values", &len);
    len /= sizeof(u32);

    dev_info(&pdev->dev, "threshold = %u\n", threshold);
    dev_info(&pdev->dev, "label = %s\n", label);
    dev_info(&pdev->dev, "values = ");
    for (i = 0; i < len; i++)
        pr_cont("%d ", be32_to_cpu(vals[i]));
    pr_cont("\n");

    for_each_child_of_node(np, child) {
        of_property_read_u32(child, "speed", &speed);
        of_property_read_string(child, "name", &child_name);

        dev_info(&pdev->dev, "Child found: speed = %u, name = %s\n",
                 speed, child_name);
    }

    return 0;
}

static void my_remove(struct platform_device *pdev)
{
    dev_info(&pdev->dev, "device removed\n");
}

static const struct of_device_id my_match[] = {
    { .compatible = "you,my-device" },
    {},
};

MODULE_DEVICE_TABLE(of, my_match);

static struct platform_driver my_driver = {
    .probe = my_probe,
    .remove = my_remove,
    .driver = {
        .name = "my_virtual_driver",
        .of_match_table = my_match,
    },
};

module_platform_driver(my_driver);

MODULE_LICENSE("GPL");
