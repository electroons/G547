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
	{ 0x9d35aeec, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x891b08c3, __VMLINUX_SYMBOL_STR(pci_dev_put) },
	{ 0xc56e920c, __VMLINUX_SYMBOL_STR(register_netdev) },
	{ 0xca37d94, __VMLINUX_SYMBOL_STR(alloc_etherdev_mqs) },
	{ 0x30c58d84, __VMLINUX_SYMBOL_STR(pci_enable_device) },
	{ 0x246849de, __VMLINUX_SYMBOL_STR(pci_get_device) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "02E7E4138CF88D1AB8AB846");
