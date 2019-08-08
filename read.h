#ifndef KERNEL_READ_H
#define KERNEL_READ_H

/**
 * "Read" filesystem operation prototype
 * Macros __user marks buf parameter as its came from user space (need additional check)
 * lbuf - buffer size
 * ppos - offset inside the buffer, that we use to read a data
 */
static ssize_t spdl_chr_dev_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos){

//device buffer
char *kbuf = file->private_data;

//Копируем байты из буфера в пространство пользователя
//Копируем байты из буфера в пространство пользователя
int nbytes = lbuf - copy_to_user(buf, kbuf + *ppos, lbuf);
*ppos += nbytes;

printk(KERN_INFO "+ Reading device %s nbytes = %d, ppos = %d \n", SPDL_DEV_NAME, nbytes, (int)*ppos);

return nbytes;
}

#endif //KERNEL_READ_H
