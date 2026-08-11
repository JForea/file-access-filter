#include "kprobe_helpers.h"

#include <linux/kprobes.h>

static struct kprobe kp = {
    .symbol_name = "do_sys_openat2",
};

static int pre_handler(struct kprobe* p, struct pt_regs *regs) {
    return 0;
}

int kprobe_init(void) {
    int ret;
    kp.pre_handler = pre_handler;

    ret = register_kprobe(&kp);

    if (ret < 0) {
        printk(KERN_INFO "register_kprobe failed, returned %d.\n", ret);
        return ret;
    }

    printk(KERN_INFO "Planted kprobe at %p\n", kp.addr);

    return 0;
}

void kprobe_exit(void) {
    unregister_kprobe(&kp);
    printk(KERN_INFO "kprobe at %p unregistered\n", kp.addr);
}
