#include "ftrace_helpers.h"
#include "rule_list.h"
#include "kallsyms_compatibility.h"

#include <linux/ftrace.h>
#include <linux/kallsyms.h>

#define MAX_LENGTH 4096

struct ftrace_hook {
    const char *name; /* Name of the kernel function to hook */
    void *function; /* Pointer to provided replacement function */
    void *original; /* Pointer to a variable that will store the original function */

    unsigned long address; /* Resolved address of the original kernel function*/
    struct ftrace_ops ops; /* ftrace configuration and callback state */
};

static asmlinkage long (*real_do_sys_openat2)(
    int dfd, 
    const char __user *filename,
	struct open_how *how
);

/*
* In this function main logic is being executed.
* This function is called before real do_sys_openat2().
*/
static asmlinkage long fh_do_sys_openat2(
    int dfd, 
    const char __user *filename,
	struct open_how *how
) {
    char *path;
    int copied;

    pr_debug("real_do_sys_openat2() called: filename=%p", filename);

    path = kmalloc(MAX_LENGTH, GFP_KERNEL);
    if (!path) {
        return -ENOMEM;
    }

    copied = strncpy_from_user(path, filename, MAX_LENGTH);
    if (copied < 0) {
        kfree(path);
        return copied;
    }

    if (does_match_any_rule(path)) {
        kfree(path);
        return -EACCES;
    }

    kfree(path);
    return real_do_sys_openat2(dfd, filename, how);
}

static struct ftrace_hook hooked_functions[] = {
    {
        .name = "do_sys_openat2",
        .function = fh_do_sys_openat2,
        .original = &real_do_sys_openat2
    }
};

/*
* Finds the address of original function.
*/
static int resolve_hook_address(struct ftrace_hook *hook) {
/*
* Small hack to enable function kallsyms_lookup_name_t(),
* which is deprecated in modern kernel versions.
*/
#ifdef KPROBE_LOOKUP
    typedef unsigned long (*kallsyms_lookup_name_t)(const char *name);
    kallsyms_lookup_name_t kallsyms_lookup_name;
    register_kprobe(&kp);
    kallsyms_lookup_name = (kallsyms_lookup_name_t) kp.addr;
    unregister_kprobe(&kp);
#endif

    hook->address = kallsyms_lookup_name(hook->name);

    if (!hook->address) {
        pr_debug("unresolved symbol: %s\n", hook->name);
        return -ENOENT;
    }

    *((unsigned long*) hook->original) = hook->address;

    return 0;
}

/*
* This function is called before executing fh_func().
*/
static void notrace fh_ftrace_thunk(
    unsigned long ip, 
    unsigned long parent_ip,
    struct ftrace_ops *ops,
    struct ftrace_regs *regs
) {
    struct ftrace_hook *hook = container_of(ops, struct ftrace_hook, ops);

    /*
    * Check if we are calling fh_func() from this module.
    * If we are not then it'll be OK, proceed the call.
    */
    if (!within_module(parent_ip, THIS_MODULE)) {
        ftrace_regs_set_instruction_pointer(
            regs,
            (unsigned long)hook->function
        );
    }
}

static int fh_install_hook(struct ftrace_hook *hook) {
    int err;

    err = resolve_hook_address(hook);
    if (err) {
        return err;
    }

    hook->ops.func = fh_ftrace_thunk; /* Register callback function */
    hook->ops.flags = 
        FTRACE_OPS_FL_SAVE_REGS |   /* Save registers before invoking the callback */
        FTRACE_OPS_FL_RECURSION |   /* Prevent recursive callback invocation */
        FTRACE_OPS_FL_IPMODIFY;     /* Allow the callback to modify the instruction pointer */

    err = ftrace_set_filter_ip(&hook->ops, hook->address, 0, 0);
    if (err) {
        pr_debug("ftrace_set_filter_ip() failed: %d\n)", err);
        return err;
    }

    err = register_ftrace_function(&hook->ops);
    if (err) {
        pr_debug("register_ftrace_function() failed: %d\n", err);

        ftrace_set_filter_ip(&hook->ops, hook->address, 1, 0);

        return err;
    }

    return 0;
}

static void fh_remove_hook(struct ftrace_hook *hook) {
    int err;

    err = unregister_ftrace_function(&hook->ops);
    if (err) {
        pr_debug("unregister_ftrace_function() failed: %d\n", err);
    }

    err = ftrace_set_filter_ip(&hook->ops, hook->address, 1, 0);
    if (err) {
        pr_debug("ftrace_set_filter_ip() failed: %d\n", err);
    }
}

int fh_init(void) {
    for (int i = 0; i < ARRAY_SIZE(hooked_functions); ++i) {
        int err;
        
        err = fh_install_hook(&hooked_functions[i]);

        if (err) {
            for (; i >= 0; --i) {
                fh_remove_hook(&hooked_functions[i]);
            }

            return err;
        }
    }

    return 0;
}

void fh_exit(void) {
    for (int i = 0; i < ARRAY_SIZE(hooked_functions); ++i) {
        fh_remove_hook(&hooked_functions[i]);
    }
}
