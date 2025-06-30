#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0xcb789b11, "__platform_driver_register" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x39d0f19b, "_dev_info" },
	{ 0xdbd6ca4c, "platform_driver_unregister" },
	{ 0xc46f4805, "of_property_read_variable_u32_array" },
	{ 0x69fd7ff5, "of_property_read_string" },
	{ 0x53dffb37, "of_get_property" },
	{ 0x92997ed8, "_printk" },
	{ 0xe771f100, "of_get_next_child" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xbbb36cd0, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Cyou,my-device");
MODULE_ALIAS("of:N*T*Cyou,my-deviceC*");

MODULE_INFO(srcversion, "865A718E357F9AAD3D54776");
