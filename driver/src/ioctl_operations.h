#ifndef IOCTL_OPERATION_H
#define IOCTL_OPERATION_H

#include <linux/ioctl.h>

/*
* Number which is used for IOCTL operations.
* This number differs driver from others.
*/
#define FAD_MAGIC 'f'

#define FAD_ADD _IOW(FAD_MAGIC, 0, char*)
#define FAD_DEL _IOW(FAD_MAGIC, 1, size_t)

#define FAD_CLEAR _IO(FAD_MAGIC, 0)

#endif