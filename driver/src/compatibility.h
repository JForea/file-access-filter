/*
* This header is used for unit-testing some of the functions in user-space.
*/
#ifndef COMPATIBILITY_H
#define COMPATIBILITY_H

#ifdef __KERNEL__

#include <linux/version.h>
#include <linux/string.h>
#include <linux/types.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,7,0)
#define KPROBE_LOOKUP 1
#include <linux/kprobes.h>
static struct kprobe kp = {
    .symbol_name = "kallsyms_lookup_name"
};
#endif

#else

#include <string.h>
#include <stdbool.h>

#endif

#endif