#include <linux/init.h>
#include <linux/module.h>
#include <linux/device/class.h>
#include <linux/device.h>

#include "fad_state.h"
#include "device_handlers.h"
#include "ftrace_helper.h"
#include "rule_list.h"

static const char *DEVICE_NAME = "fad";
static dev_t devt;

static struct class *cls;

static fad_state state;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JForea");
MODULE_DESCRIPTION("Linux kernel module for restricting access to user-specified files.");

static struct file_operations fops = {
    .open = fad_open,
    .unlocked_ioctl = fad_ioctl,
    .release = fad_release,
};

static char *fad_devnode(const struct device *dev, umode_t *mode) {
    /*
    * Only root user can access device.
    */
    if (mode)
        *mode = 0600;

    return NULL;
}

static int __init fad_init(void) {
    int err;
    struct device *dev;

    rule_list_init();

    err = fh_init();
    if (err) {
        return err;
    }

    state.major = register_chrdev(0, DEVICE_NAME, &fops);
    if (state.major < 0) { 
        printk(KERN_ALERT "FAD: Failed to register major number.\n"); 
        return state.major; 
    }
    devt = MKDEV(state.major, 0);

    /*
    * In older kernel versions the signature for class_create
    * was like class_create(THIS_MODULE, DEVICE_NAME).
    * Possibly need to implement it later.
    */
    cls = class_create(DEVICE_NAME);
    if (IS_ERR(cls)) {
        printk(KERN_ALERT "FAD: Failed to create device class.\n");
        unregister_chrdev(state.major, DEVICE_NAME);
        return PTR_ERR(cls);
    }
    cls->devnode = fad_devnode;
    
    dev = device_create(cls, NULL, devt, NULL, DEVICE_NAME);
    if (IS_ERR(dev)) {
        printk(KERN_ALERT "FAD: Failed to create device class.\n");
        class_destroy(cls);
        unregister_chrdev(state.major, DEVICE_NAME);
        return PTR_ERR(dev);
    }

    printk(KERN_INFO "FAD: File access driver started. Device character registered with major %d\n", state.major);
    return 0;
}

static void __exit fad_exit(void) {
    device_destroy(cls, devt);
    class_destroy(cls);
    unregister_chrdev(state.major, DEVICE_NAME);
    fh_exit();
    remove_all_rules();
    printk(KERN_INFO "FAD: File access driver stopped.\n");
}

module_init(fad_init);

module_exit(fad_exit);