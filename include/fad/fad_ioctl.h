#ifndef FAD_IOCTL_RULE_H
#define FAD_IOCTL_RULE_H

#include <linux/ioctl.h>
#include <linux/types.h>

/*
* Number which is used for IOCTL operations.
* This number differs driver from others.
*/
#define FAD_MAGIC 'f'

typedef struct {
    __u64 mask;
    __u32 length;
    /*
    * IOCTL structures should have their members naturally 
    * aligned, or explicit reserved fields added in place 
    * of the implicit padding.
    */
    __u32 reserved;
} fad_ioctl_rule;

#define FAD_ADD _IOW(FAD_MAGIC, 0, fad_ioctl_rule)
#define FAD_DEL _IOW(FAD_MAGIC, 1, fad_ioctl_rule)

#define FAD_CLEAR _IO(FAD_MAGIC, 2)

#endif