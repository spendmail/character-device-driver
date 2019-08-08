#!/bin/bash

sudo chmod a+rw /dev/spdl_chrdev && \
echo "test" > /dev/spdl_chrdev
