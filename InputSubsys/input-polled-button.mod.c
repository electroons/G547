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

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x7377b0b2, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xb721c93f, __VMLINUX_SYMBOL_STR(platform_driver_unregister) },
	{ 0xb5c472f8, __VMLINUX_SYMBOL_STR(__platform_driver_register) },
	{ 0xcae861dd, __VMLINUX_SYMBOL_STR(input_event) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x3118fcc9, __VMLINUX_SYMBOL_STR(input_register_polled_device) },
	{ 0x553ccd5a, __VMLINUX_SYMBOL_STR(input_allocate_polled_device) },
	{ 0x14aca025, __VMLINUX_SYMBOL_STR(input_free_polled_device) },
	{ 0x11016e8f, __VMLINUX_SYMBOL_STR(input_unregister_polled_device) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=input-polldev";


MODULE_INFO(srcversion, "2C15561982ADFA0769FE261");
