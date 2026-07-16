#ifndef KALLSYMS_COMPATIBILITY_H
#define KALLSYMS_COMPATIBILITY_H

#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,7,0)
#define KPROBE_LOOKUP
#include <linux/kprobes.h>
static struct kprobe kp = {
    .symbol_name = "kallsyms_lookup_name"
};
#endif

#endif