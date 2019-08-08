#ifndef KERNEL_DEFINES_H
#define KERNEL_DEFINES_H

/**
 * Module name that visible to user
 */
#define SPDL_DEV_NAME "spdl_chr_dev"

/**
 * Buffer size, that user will use
 * (10 pages of memory)
 */
#define KBUF_SIZE (size_t) ((10) * PAGE_SIZE)

#endif //KERNEL_DEFINES_H
