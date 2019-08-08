#ifndef KERNEL_CLEAN_H
#define KERNEL_CLEAN_H

/**
 * "Release" filesystem operation prototype
 */
static int spdl_chr_dev_release(struct inode *inode, struct file *file){

    char *kbuf = file->private_data;

    printk(KERN_INFO "+ Releasing device %s\n", SPDL_DEV_NAME);

    //Release the memory, allocated for the device buffer
    printk(KERN_INFO "+ Free buffer\n");

    //Checking that memory was allocated beforehand
    if(kbuf){
        kfree(kbuf);
    }

    //Setting null to prevent any mistakes
    kbuf = NULL;
    file->private_data = NULL;

    return 0;
}

#endif //KERNEL_CLEAN_H
