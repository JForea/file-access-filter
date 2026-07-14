#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JForea");
MODULE_DESCRIPTION("Linux kernel module for restricting access to user-specified files.");

static int __init fad_init(void) {
    printk(KERN_INFO "File access driver started.");
    return 0;
}

static void __exit fad_exit(void) {
    printk(KERN_INFO "File access driver stopped.");
}

module_init(fad_init);

module_exit(fad_exit);