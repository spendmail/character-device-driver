#ifndef KERNEL_WRITE_H
#define KERNEL_WRITE_H

/**
  * "Write" filesystem operation prototype
  */
static ssize_t spdl_chr_dev_write(struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos){

    //device buffer
    char *kbuf = file->private_data;

    int nbytes = lbuf - copy_from_user(kbuf + *ppos, buf, lbuf);
    *ppos += nbytes;

    printk(KERN_INFO "+ Writing into device %s nbytes = %d, ppos = %d \n", SPDL_DEV_NAME, nbytes, (int)*ppos);

    return nbytes;
}

#endif //KERNEL_WRITE_H
