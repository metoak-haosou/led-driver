#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x6c9dadc8, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x2ab3cc9d, __VMLINUX_SYMBOL_STR(__release_region) },
	{ 0xedc03953, __VMLINUX_SYMBOL_STR(iounmap) },
	{ 0x85f74b00, __VMLINUX_SYMBOL_STR(iomem_resource) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0xbf9d756, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x375cdb51, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x79c5a9f0, __VMLINUX_SYMBOL_STR(ioremap) },
	{ 0x9416e1d8, __VMLINUX_SYMBOL_STR(__request_region) },
	{ 0xb7754f2a, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x8bc4f6e5, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0xb74f0973, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x5d534a5b, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0x220c9794, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0xe5ee3a5c, __VMLINUX_SYMBOL_STR(cdev_alloc) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0xd8e484f0, __VMLINUX_SYMBOL_STR(register_chrdev_region) },
	{ 0x822137e2, __VMLINUX_SYMBOL_STR(arm_heavy_mb) },
	{ 0x353e3fa5, __VMLINUX_SYMBOL_STR(__get_user_4) },
	{ 0xefd6cf06, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr0) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "62A8880542F2B5ADDD87517");
