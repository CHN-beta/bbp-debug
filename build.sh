#!/bin/bash

cd /home/chn/Desktop/openwrt-sdk-18.06.5-x86-64_gcc-7.3.0_musl.Linux-x86_64
cp -rf ../bbp-debug package/
make deconfig
make package/bbp-debug/compile V=sc
scp bin/targets/x86/64/packages/kmod-bbp-debug_4.14.151-1_x86_64.ipk root@192.168.56.2:~/


