#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
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

#ifdef CONFIG_MITIGATION_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const char ____versions[]
__used __section("__versions") =
	"\x14\x00\x00\x00\xbb\x6d\xfb\xbd"
	"__fentry__\0\0"
	"\x10\x00\x00\x00\xf9\x82\xa4\xf9"
	"msleep\0\0"
	"\x14\x00\x00\x00\x44\x43\x96\xe2"
	"__wake_up\0\0\0"
	"\x10\x00\x00\x00\xba\x0c\x7a\x03"
	"kfree\0\0\0"
	"\x1c\x00\x00\x00\x63\xa5\x03\x4c"
	"random_kmalloc_seed\0"
	"\x18\x00\x00\x00\x98\x04\xd2\xf2"
	"kmalloc_caches\0\0"
	"\x20\x00\x00\x00\xfe\x6e\xe0\xb2"
	"__kmalloc_cache_noprof\0\0"
	"\x14\x00\x00\x00\xd3\x85\x33\x2d"
	"system_wq\0\0\0"
	"\x18\x00\x00\x00\x36\xf2\xb6\xc5"
	"queue_work_on\0\0\0"
	"\x20\x00\x00\x00\x5d\x7b\xc1\xe2"
	"__SCT__might_resched\0\0\0\0"
	"\x18\x00\x00\x00\x75\x79\x48\xfe"
	"init_wait_entry\0"
	"\x14\x00\x00\x00\x51\x0e\x00\x01"
	"schedule\0\0\0\0"
	"\x20\x00\x00\x00\x95\xd4\x26\x8c"
	"prepare_to_wait_event\0\0\0"
	"\x14\x00\x00\x00\xbf\x0f\x54\x92"
	"finish_wait\0"
	"\x1c\x00\x00\x00\xcb\xf6\xfd\xf0"
	"__stack_chk_fail\0\0\0\0"
	"\x10\x00\x00\x00\x7e\x3a\x2c\x12"
	"_printk\0"
	"\x1c\x00\x00\x00\xca\x39\x82\x5b"
	"__x86_return_thunk\0\0"
	"\x18\x00\x00\x00\x34\x61\x23\x68"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "73CC9919049419DE36B57A2");
