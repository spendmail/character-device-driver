#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/cdev.h>

/**
 * Global variables are static, not to export any names outside
 */

/**
 * Identifier of the first device in the chain
 */
static dev_t first;

/**
 * Device counter
 */
static unsigned int count = 1;

/**
 * Major device id (Vendor Id).
 * Replaced 700 with 500 because of the error:
 * CHRDEV "spdl_chr_dev" major requested (700) is greater than the maximum (512)
 */
static int dev_major = 500;

/**
 * Minor device id (Vendor Id)
 */
static int dev_minor = 0;

/**
 * Device structure,
 * contain all operations,
 * we can execute over device
 */
static struct cdev *cdev_struct;

/**
 * Module name that visible to user
 */
#define SPDL_DEV_NAME "spdl_chr_dev"

/**
 * Buffer size, that user will use
 * (10 pages of memory)
 */
#define KBUF_SIZE (size_t) ((10) * PAGE_SIZE)

/**
 * Dynamic device node creating param
 */
static struct class *spdl_class;

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

static loff_t spdl_chr_dev_lseek(struct file *file, loff_t offset, int orig){

    loff_t testpos;

    switch(orig){
        case SEEK_SET:
            testpos = offset;
            break;
        case SEEK_CUR:
            testpos = file->f_pos + offset;
            break;
        case SEEK_END:
            testpos = KBUF_SIZE + offset;
            break;
        default:
            return ~EINVAL;
    }

    testpos = testpos < KBUF_SIZE ? testpos : KBUF_SIZE;
    testpos = testpos >= 0 ? testpos : 0;
    file->f_pos = testpos;

    printk(KERN_INFO "+ Seeking to %ld position\n, ", (long)testpos);

    return testpos;
}

/**
 * Definition of a structure for the filesystem operations,
 * that we use to store pointers at created filesystem functions
 */
static const struct file_operations mycdev_fops = {

   .owner = THIS_MODULE,
   .read = spdl_chr_dev_read,
   .write = spdl_chr_dev_write,
   .open = spdl_chr_dev_open,
   .release = spdl_chr_dev_release,
   .llseek = spdl_chr_dev_lseek
};

/**
 * Initialization functions
 */
static int __init init_chrdev( void ) {
    printk( KERN_INFO " + Hello, loading!\n" );

    /**
     * @TODO: error checking when allocating memory
     */

    //First device initialization
    //Macros MKDEV user for creating an inode in filesystem
    first = MKDEV (dev_major, dev_minor);

    //Region registration - set of identifiers for available instances of our device
    register_chrdev_region(first, count, SPDL_DEV_NAME);

    /**
     * @TODO: error checking during devices registration
     */

    //Memory allocating for the structure, that includes all device attributes
    cdev_struct = cdev_alloc();

    //Structure initialization, referred with filesystem operations
    //&mycdev_fops - pointer at the filesystem operations structure
    cdev_init(cdev_struct, &mycdev_fops);

    //Attach a devices tree (1 device)
    cdev_add(cdev_struct, first, count);

    //Dynamic creating a device file
    spdl_class = class_create(THIS_MODULE, "spdl_class");

    /**
     * @TODO: error checking for the value, that class_create returns
     */

    device_create(spdl_class, NULL, first, "%s", "spdl_chrdev");

    printk(KERN_INFO "+ Creating device class %s\n", SPDL_DEV_NAME);

    return 0;
}

static void __exit cleanup_chrdev( void ) {

    printk(KERN_INFO "+ Leaving!\n" );

    //Device unregistrating
    device_destroy(spdl_class, first);
    class_destroy(spdl_class);

    //Device deinitializing
    if(cdev_struct){
        cdev_del(cdev_struct);
    }
    unregister_chrdev_region(first, count);
}

module_init( init_chrdev );
module_exit( cleanup_chrdev );

/**
 * License
 */
MODULE_LICENSE( "GPL" );

