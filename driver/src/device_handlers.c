#include "device_handlers.h"
#include "ioctl_operations.h"

int fad_open(struct inode *inodep, struct file *filep) { 
      printk(KERN_INFO "FAD: Device opened.\n"); 
      return 0; 
}

long fad_ioctl(struct file *filep, unsigned int cmd, unsigned long arg) { 
    printk(KERN_INFO "FAD: IOCTL command received - CMD: %u ARG: %lu.\n", cmd, arg);

    // switch (cmd) {
    // case FAD_ADD:
        
    // }

    return 0; 
}

int fad_release(struct inode *inodep, struct file *filep) { 
      printk(KERN_INFO "FAD: Device closed.\n"); 
      return 0; 
}
