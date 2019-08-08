
# Character device kernel module

## Commands:
* Make the kernel module

    `make`

* Remove unnecessary files

    `make clean`

* To launch monitor

    `dmesg -w`

    or

    `tail -n0 -f /var/log/syslog`

* Install kernel module

    `sudo insmod chrdev.ko`

* Test if module exists

    `lsmod | grep chrdev`

* Test if module is loaded and alive in kernel system 

    `cat /proc/modules | grep chrdev`

* Setting write/read permissions

    `sudo chmod a+rw /dev/spdl_chrdev`

    `ls -al /dev/spdl_chrdev`

* Writing into device

    `echo "test" > /dev/spdl_chrdev`

* Unload module from kernel

    `sudo rmmod chrdev.ko`

    or

    `sudo rmmod chrdev`

* Deleting node

    `sudo rm /dev/spdl_chrdev`

* Fast commands

    `./bin/run.sh`

    `./bin/write.sh`

    `./bin/stop.sh`
