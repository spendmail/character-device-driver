#!/bin/bash

make clean && \
make && \
sudo insmod chrdev.ko
