#ifndef KERNEL_SEEK_H
#define KERNEL_SEEK_H

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

#endif //KERNEL_SEEK_H
