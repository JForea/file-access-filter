#ifndef DEVICE_HANDLERS_H
#define DEVICE_HANDLERS_H

#include <linux/fs.h>
#include <linux/uaccess.h>

int fad_open(struct inode *inodep, struct file *filep);
int fad_release(struct inode *inodep, struct file *filep);
long fad_ioctl(struct file *filep, unsigned int cmd, unsigned long arg);

#endif