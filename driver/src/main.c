#include <linux/init.h>
#include <linux/module.h>

#include "fad_state.h"
#include "device_handlers.h"

const char* DEVICE_NAME = "fad";

static fad_state state;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JForea");
MODULE_DESCRIPTION("Linux kernel module for restricting access to user-specified files.");

static struct file_operations fops = {
    .open = fad_open,
    .unlocked_ioctl = fad_ioctl,
    .release = fad_release,
};

static int __init fad_init(void) {
    state.major = register_chrdev(0, DEVICE_NAME, &fops);
    if (state.major < 0) { 
        printk(KERN_ALERT "FAD: Failed to register major number.\n"); 
        return state.major; 
    }

    
    printk(KERN_INFO "FAD: File access driver started.");
    return 0;
}

static void __exit fad_exit(void) {
    unregister_chrdev(state.major, DEVICE_NAME);
    printk(KERN_INFO "FAD: File access driver stopped.");
}

module_init(fad_init);

module_exit(fad_exit);