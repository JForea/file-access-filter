#include "kprobe_helpers.h"
#include "rule_list.h"

#include <linux/kprobes.h>

#define MAX_PATH_SIZE 256

static struct kprobe kp = {
    .symbol_name = "do_sys_openat2",
};

static noinline long do_sys_openat2_replacement(
    int dfd,
    const char __user *filename,
    struct open_how *how
) {
    return -EACCES;
}

static int pre_handler(struct kprobe* p, struct pt_regs *regs) {
    const char __user *u_filename;
    char k_filename[MAX_PATH_SIZE];
    long n;

    u_filename = regs->si;

    n = strncpy_from_user(k_filename, u_filename, MAX_PATH_SIZE);
    if (n < 0) {
        pr_info("failed to read filename: %ld\n", n);
        return 0;
    }

    if (n >= sizeof(k_filename)) {
        k_filename[sizeof(k_filename) - 1] = '\0';
    }

    if (does_match_any_rule(k_filename)) {
        instruction_pointer_set(
            regs,
            (unsigned long)do_sys_openat2_replacement
        );
        
        return 1;
    }

    return 0;
}

NOKPROBE_SYMBOL(pre_handler);
NOKPROBE_SYMBOL(do_sys_openat2_replacement);

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
