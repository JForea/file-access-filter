#include "device_handlers.h"
#include "rule_list.h"

#include <fad/fad_ioctl.h>

#define FAD_MAX_MASK_LENGTH 4096

/*
* May return ERR_PTR as error.
*/
static char *parse_mask(unsigned long arg) {
	fad_ioctl_rule rule;

	if (copy_from_user(
		&rule, 
		(const void __user *)arg, 
		sizeof(rule)
	) != 0) {
		return ERR_PTR(-EFAULT);
	} 
	
	if (rule.length > FAD_MAX_MASK_LENGTH) {
		return ERR_PTR(-E2BIG);
	}

	if (rule.length == 0 || !rule.mask) {
		return ERR_PTR(-EINVAL);
	}

	return memdup_user_nul(
		u64_to_user_ptr(rule.mask), 
		rule.length
	);
}

int fad_open(struct inode *inodep, struct file *filep) { 
      printk(KERN_INFO "FAD: Device opened.\n"); 
      return 0; 
}

long fad_ioctl(struct file *filep, unsigned int cmd, unsigned long arg) { 
    printk(KERN_INFO "FAD: IOCTL command received - CMD: %u ARG: %lu.\n", cmd, arg);

    switch (cmd) {
    case FAD_ADD: {
		char* mask = parse_mask(arg);
		int res;

		if (IS_ERR(mask)) {
			return PTR_ERR(mask);
		}

		res = add_rule(mask);
		kfree(mask);

		return res;
	}

	case FAD_DEL: {
		char* mask = parse_mask(arg);
		int res;

		if (IS_ERR(mask)) {
			return PTR_ERR(mask);
		}

		res = remove_rule(mask);
		kfree(mask);

		return res;
	}

	case FAD_CLEAR: {
		remove_all_rules();

		return 0;
	}

	default: {
		return -ENOTTY;
	}
    }
}

int fad_release(struct inode *inodep, struct file *filep) { 
	printk(KERN_INFO "FAD: Device closed.\n"); 
	return 0; 
}
