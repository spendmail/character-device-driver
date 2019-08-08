#ifndef KERNEL_OPEN_H
#define KERNEL_OPEN_H

/**
 * "Open" filesystem operation prototype
 */
static int spdl_chr_dev_open(struct inode *inode, struct file *file){

    //New device buffer for each opening
    //(allocating memory for them in kernel)
    char *kbuf = kmalloc(KBUF_SIZE, GFP_KERNEL);

    //Resource counter
    static int counter = 0;

    //Saving an address of buffer in file structure
    file->private_data = kbuf;

    printk(KERN_INFO "+ Opening device %s\n", SPDL_DEV_NAME);

    counter++;

    //Printing resource counter
    printk(KERN_INFO "+ Counter: %d\n", counter);

    //Printing users counter (from the kernel point)
    printk(KERN_INFO "+ Module refcounter: %d\n", module_refcount(THIS_MODULE));

    return 0;
}

#endif //KERNEL_OPEN_H
