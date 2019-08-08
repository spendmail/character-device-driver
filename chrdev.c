#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include "defines.h"
#include "open.h"
#include "release.h"
#include "read.h"
#include "write.h"
#include "seek.h"

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
 * Dynamic device node creating param
 */
static struct class *spdl_class;

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
