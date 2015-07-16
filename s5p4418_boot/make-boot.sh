#!/bin/sh
cp ./arch/arm/boot/uImage ./s5p4418_boot/boot
./s5p4418_boot/make_ext4fs -s -l 18M -L boot ./s5p4418_boot/boot.img ./s5p4418_boot/boot
